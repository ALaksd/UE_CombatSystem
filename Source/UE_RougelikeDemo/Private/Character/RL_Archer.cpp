// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RL_Archer.h"
#include"Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/RL_RowCompoment.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ARL_Archer::ARL_Archer()
{
	BowComponent = CreateDefaultSubobject<URL_RowCompoment>(TEXT("BowComponent"));
	AimTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("AimTimeline"));
}

void ARL_Archer::BeginPlay()
{
	Super::BeginPlay();
	InitialFov = FllowCamera->FieldOfView;
	InitialCameraOffest = CameraBoom->SocketOffset;
	AimFov = 60;
	AimCameraOffest = FVector(100, 60, 80);

	//��ʼ��ʱ����
	if (AimCurve)
	{
		FOnTimelineFloat TimelineProcess;
		TimelineProcess.BindUFunction(this, FName("OnAimTimelineUpdate"));
		AimTimeline->AddInterpFloat(AimCurve, TimelineProcess);

		//����ʱ����������ظ�
		AimTimeline->SetLooping(false);
	}
}

void ARL_Archer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ARL_Archer::SetAimCamera);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ARL_Archer::ResetAimCamera);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, BowComponent, &URL_RowCompoment::PullBowBegin);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, BowComponent, &URL_RowCompoment::FireArrowBegin);
	}
}

void ARL_Archer::OnAimTimelineUpdate(float Value)
{
	//��ֵFov
	float NewFov = FMath::Lerp(InitialFov, AimFov, Value);
	FllowCamera->SetFieldOfView(NewFov);

	//��ֵ�����ƫ��
	FVector NewOffset = FMath::Lerp(InitialCameraOffest, AimCameraOffest, Value);
	CameraBoom->SocketOffset = NewOffset;
}

void ARL_Archer::SetAimCamera()
{
	BowComponent->AimBegin();
	if (AimTimeline)
	{
		AimTimeline->Play();  // ���򲥷�ʱ����
	}
	//UpdateMovementState(EMovementState::Walking);
}

void ARL_Archer::ResetAimCamera()
{
	BowComponent->AimEnd();
	if (AimTimeline)
	{
		AimTimeline->Reverse();  // ���򲥷�ʱ����
	}
	//UpdateMovementState(EMovementState::Jogging);
}

void ARL_Archer::Fire()
{
	BowComponent->FireArrowBegin();
}
