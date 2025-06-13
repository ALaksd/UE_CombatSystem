// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableDoorActor.h"

#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"

AInteractableDoorActor::AInteractableDoorActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	DoorLeftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeftMesh"));
	DoorRightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorRightMesh"));
	DoorLeftMesh->SetupAttachment(InteractCollision);
	DoorRightMesh->SetupAttachment(InteractCollision);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(InteractCollision);

	OpenTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("OpenTimeline"));
}

void AInteractableDoorActor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableDoorActor::PushDoor()
{
	InitialLRotation = DoorLeftMesh->GetRelativeRotation();
	TargetLRotation = InitialLRotation + FRotator(0.f, -90.f, 0.f);

	InitialRRotation = DoorRightMesh->GetRelativeRotation();
	TargetRRotation = InitialRRotation + FRotator(0.f, 90.f, 0.f);

	if (OpenCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleTimelineProgress1"));
		OpenTimeline->AddInterpFloat(OpenCurve, ProgressFunction);
		OpenTimeline->SetLooping(false);
	}
	
	if (OpenCurve && OpenTimeline && !OpenTimeline->IsPlaying())
	{
		OpenTimeline->PlayFromStart();
	}

	InteractCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (PassCheck)
	{
		PassCheck->SetActorEnableCollision(false);
	}
}

void AInteractableDoorActor::TranslateDoor(FVector Offset)
{
	InitialLocation = DoorMesh->GetRelativeLocation();
	TargetLocation = InitialLocation + Offset;

	if (OpenCurve)
	{
		FOnTimelineFloat ProgressFunction;
		ProgressFunction.BindUFunction(this, FName("HandleTimelineProgress2"));
		OpenTimeline->AddInterpFloat(OpenCurve, ProgressFunction);
		OpenTimeline->SetLooping(false);
	}
	
	if (OpenCurve && OpenTimeline && !OpenTimeline->IsPlaying())
	{
		OpenTimeline->PlayFromStart();
	}

	InteractCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (PassCheck)
	{
		PassCheck->SetActorEnableCollision(false);
	}
}

void AInteractableDoorActor::HandleTimelineProgress1(float Value)
{
	FRotator NewLRotation = UKismetMathLibrary::RLerp(InitialLRotation, TargetLRotation, Value, true);
	DoorLeftMesh->SetRelativeRotation(NewLRotation);

	FRotator NewRRotation = UKismetMathLibrary::RLerp(InitialRRotation, TargetRRotation, Value, true);
	DoorRightMesh->SetRelativeRotation(NewRRotation);
}

void AInteractableDoorActor::HandleTimelineProgress2(float Value)
{
	FVector NewLocation = FMath::Lerp(InitialLocation, TargetLocation, Value);
	DoorMesh->SetRelativeLocation(NewLocation);
}
