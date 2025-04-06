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

//开始瞄准并生成弓箭
void URL_RowCompoment::AimBegin()
{
	IsAiming = true;
	if (AttachCharacter)
	{
		//设置使用控制器所需的旋转
		AttachCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		//设置将转向朝至运动
		AttachCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		//设置角色旋转速率
		AttachCharacter->GetCharacterMovement()->RotationRate = AimRotaion;
	}

	//设置弓的状态
	Bow->BowState = EBowState::Aim;
	//生成弓箭
	SpawnArrow();

	// 广播瞄准开始的事件
	OnAimBegin.Broadcast();
}

//结束瞄准并销毁弓箭
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

	//销毁弓箭
	DestroyArrow();

	// 广播瞄准结束的事件
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
	//没有拉弓并且正在瞄准时
	if (!IsPulling && IsAiming)
	{
		if (!IsWaitingToPulling)
		{
			IsPulling = true;
			PullBowDuration = 0.0f;

			//设置角色速度
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

	// 停止定时器
	GetWorld()->GetTimerManager().ClearTimer(PullBowTimeHandle);
	
	Bow->PullBowEnd();

	OnPullEnd.Broadcast();
}

void URL_RowCompoment::UpdatePullBowDuration()
{
	PullBowDuration += PullIncreasTime;
	OnPullBegin.Broadcast(PullBowDuration, Bow->MaxPullTime);
	// 处理拉弓时长，例如发射箭或调整箭的速度等
	//UE_LOG(LogTemp, Warning, TEXT("Bow pulled for %f seconds"), PullBowDuration);
}

void URL_RowCompoment::FireArrowBegin()
{
	if (IsPulling && !IsFiring)
	{
		IsFiring = true;

		//计算拉弓力度
		float Strength = UKismetMathLibrary::NormalizeToRange(PullBowDuration, 0.0f, 1.0f);
		Strength = FMath::Clamp(Strength, 0.0f, 1.0f);
		FireArrow(Strength);
		SpawnedArrow = nullptr;
		PullBowEnd();
		 // 播放蒙太奇
		UAnimInstance* AnimInstance = AttachCharacter->GetMesh()->GetAnimInstance();
		if (AnimInstance && FireMontage)
		{
			PlayMontage(FireMontage);
			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &URL_RowCompoment::OnMontageEnded);

			// 设置蒙太奇结束时的委托
			AnimInstance->Montage_SetEndDelegate(EndDelegate);

			
		}
	}

}

void URL_RowCompoment::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 当蒙太奇播放结束或被打断时调用 FireArrowEnd
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
		//与owner分离
		SpawnedArrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		if (AttachCharacter)
		{
			//设置发射速度
			SpawnedArrow->Fire(CalculateArrowDirection(), Strength);
		}
	}
}

FVector URL_RowCompoment::CalculateArrowDirection()
{
	//获取角色相机组件
	UCameraComponent* CameraComponent = Cast<UCameraComponent>(AttachCharacter->GetComponentByClass(UCameraComponent::StaticClass()));
	if (!CameraComponent)
	{
		return FVector();
	}

	FVector Start = CameraComponent->GetComponentLocation();
	FVector Direction = CameraComponent->GetForwardVector();
	FVector End = Start + Direction * 10000.0f;

	// 射线检测命中结果
	FHitResult  HitResult;

	//进行射线检测
	FCollisionQueryParams QueryParams;
	//忽略角色自己
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
		FinalDirection = HitResult.Location - AttachCharacter->GetMesh()->GetSocketLocation(ArrowSocket);  // 从相机位置到命中点的方向
	}
	else
	{
		FinalDirection = End - AttachCharacter->GetMesh()->GetSocketLocation(ArrowSocket);  // 如果没有命中任何物体，朝前方 10000 单位的方向
	}
		
	// 归一化方向向量以确保它是单位向量
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



