// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/RL_RowCompoment.h"
#include "Weapon/RL_Arrow.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h" 

void URL_RowCompoment::BeginPlay()
{
	Super::BeginPlay();
	InitialRotaion = AttachCharacter->GetCharacterMovement()->RotationRate;
	AimRotaion = FRotator(0, 1000,0);

	if (BaseWeapon)
	{
		Bow = Cast<ARL_Bow>(BaseWeapon);
	}
}

//��ʼ��׼�����ɹ���
void URL_RowCompoment::AimBegin()
{
	IsAiming = true;
	if (AttachCharacter)
	{
		//����ʹ�ÿ������������ת
		AttachCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		//���ý�ת�����˶�
		AttachCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		//���ý�ɫ��ת����
		AttachCharacter->GetCharacterMovement()->RotationRate = AimRotaion;
	}

	//���ù���״̬
	Bow->BowState = EBowState::Aim;
	//���ɹ���
	SpawnArrow();

	// �㲥��׼��ʼ���¼�
	OnAimBegin.Broadcast();
}

//������׼�����ٹ���
void URL_RowCompoment::AimEnd()
{
	IsAiming = false;
	PullBowEnd();
	if (AttachCharacter)
	{
		AttachCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		AttachCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		AttachCharacter->GetCharacterMovement()->RotationRate = InitialRotaion;
	}

	Bow->BowState = EBowState::Idle;

	//���ٹ���
	DestroyArrow();

	// �㲥��׼�������¼�
	OnAimEnd.Broadcast();
}

void URL_RowCompoment::SpawnArrow()
{
	FActorSpawnParameters SpawnParameters;
	if (ArrowClass)
	{
		SpawnedArrow = GetWorld()->SpawnActor<ARL_Arrow>(ArrowClass, SpawnParameters);
		if (SpawnedArrow)
		{
			SpawnedArrow->AttachToComponent(AttachCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ArrowSocket);
		}
	}

}

void URL_RowCompoment::DestroyArrow()
{
	if (SpawnedArrow)
	{
		SpawnedArrow->Destroy();
	}
}

void URL_RowCompoment::PullBowBegin()
{
	//û����������������׼ʱ
	if (!IsPulling && IsAiming)
	{
		if (!IsWaitingToPulling)
		{
			IsPulling = true;
			PullBowDuration = 0.0f;

			//���ý�ɫ�ٶ�
			AttachCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.0f;
			GetWorld()->GetTimerManager().SetTimer(PullBowTimeHandle, this, &URL_RowCompoment::UpdatePullBowDuration, PullIncreasTime, true);

			Bow->PullBowStart();
		}
	}
	else
	{
		IsWaitingToPulling = true;
	}
}

void URL_RowCompoment::PullBowEnd()
{
	IsPulling = false;

	IsWaitingToPulling = false;

	PullBowDuration = 0.0f;

	AttachCharacter->GetCharacterMovement()->MaxWalkSpeed = 600.0f;

	// ֹͣ��ʱ��
	GetWorld()->GetTimerManager().ClearTimer(PullBowTimeHandle);
	
	Bow->PullBowEnd();

	OnPullEnd.Broadcast();
}

void URL_RowCompoment::UpdatePullBowDuration()
{
	PullBowDuration += PullIncreasTime;
	OnPullBegin.Broadcast(PullBowDuration, Bow->MaxPullTime);
	// ��������ʱ�������緢�������������ٶȵ�
	//UE_LOG(LogTemp, Warning, TEXT("Bow pulled for %f seconds"), PullBowDuration);
}

void URL_RowCompoment::FireArrowBegin()
{
	if (IsPulling && !IsFiring)
	{
		IsFiring = true;

		//������������
		float Strength = UKismetMathLibrary::NormalizeToRange(PullBowDuration, 0.0f, 1.0f);
		Strength = FMath::Clamp(Strength, 0.0f, 1.0f);
		FireArrow(Strength);
		SpawnedArrow = nullptr;
		PullBowEnd();
		 // ������̫��
		UAnimInstance* AnimInstance = AttachCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && FireMontage)
		{
			PlayMontage(FireMontage);
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &URL_RowCompoment::OnMontageEnded);

			// ������̫�����ʱ��ί��
			AnimInstance->Montage_SetEndDelegate(EndDelegate);

			
		}
	}

}

void URL_RowCompoment::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// ����̫�沥�Ž����򱻴��ʱ���� FireArrowEnd
	FireArrowEnd();
}

void URL_RowCompoment::FireArrowEnd()
{
	IsFiring = false;
	if (IsAiming)
	{
		AimBegin();
		if (IsWaitingToPulling)
		{
			PullBowBegin();
		}
	}
}

void URL_RowCompoment::FireArrow(float Strength)
{
	if (SpawnedArrow)
	{
		//��owner����
		SpawnedArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		if (AttachCharacter)
		{
			//���÷����ٶ�
			SpawnedArrow->Fire(CalculateArrowDirection(), Strength);
		}
	}
}

FVector URL_RowCompoment::CalculateArrowDirection()
{
	//��ȡ��ɫ������
	UCameraComponent* CameraComponent = Cast<UCameraComponent>(AttachCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
	if (!CameraComponent)
	{
		return FVector();
	}

	FVector Start = CameraComponent->GetComponentLocation();
	FVector Direction = CameraComponent->GetForwardVector();
	FVector End = Start + Direction * 10000.0f;

	// ���߼�����н��
	FHitResult  HitResult;

	//�������߼��
	FCollisionQueryParams QueryParams;
	//���Խ�ɫ�Լ�
	QueryParams.AddIgnoredActor(AttachCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);

	FVector FinalDirection;
	if (bHit)
	{
		FinalDirection = HitResult.Location - AttachCharacter->GetMesh()->GetSocketLocation(ArrowSocket);  // �����λ�õ����е�ķ���
	}
	else
	{
		FinalDirection = End - AttachCharacter->GetMesh()->GetSocketLocation(ArrowSocket);  // ���û�������κ����壬��ǰ�� 10000 ��λ�ķ���
	}
		
	// ��һ������������ȷ�����ǵ�λ����
	return FinalDirection.GetSafeNormal();
}

void URL_RowCompoment::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = AttachCharacter->GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
	}
}



