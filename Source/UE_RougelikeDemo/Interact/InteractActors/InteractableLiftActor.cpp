// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableLiftActor.h"

#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "UE_RougelikeDemo/Interact/InteractAction/LiftAction.h"

AInteractableLiftActor::AInteractableLiftActor()
{
	PrimaryActorTick.bCanEverTick = false;

	LiftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LiftMesh"));
	LiftMesh->SetupAttachment(InteractCollision);

	UpTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("UpTimeline"));
	DownTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DownTimeline"));
}

void AInteractableLiftActor::BeginPlay()
{
	Super::BeginPlay();

	if (TranslateCurve)
	{
		FOnTimelineFloat ProgressFunction1;
		FOnTimelineEvent FinishedDelegate1;
		ProgressFunction1.BindUFunction(this, FName("HandleTimelineProgressUp"));
		FinishedDelegate1.BindUFunction(this, FName("OnTimelineFinished"));
		UpTimeline->SetTimelineFinishedFunc(FinishedDelegate1);
		UpTimeline->AddInterpFloat(TranslateCurve, ProgressFunction1);
		UpTimeline->SetLooping(false);

		FOnTimelineFloat ProgressFunction2;
		FOnTimelineEvent FinishedDelegate2;
		ProgressFunction2.BindUFunction(this, FName("HandleTimelineProgressDown"));
		FinishedDelegate2.BindUFunction(this, FName("OnTimelineFinished"));
		DownTimeline->SetTimelineFinishedFunc(FinishedDelegate2);
		DownTimeline->AddInterpFloat(TranslateCurve, ProgressFunction2);
		DownTimeline->SetLooping(false);
	}
}

void AInteractableLiftActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableLiftActor::LiftRunning(FVector Offset)
{
	if (IsUp)
	{
		DownLocation = GetActorLocation();
		UpLocation = DownLocation + Offset;
		
		if (TranslateCurve && UpTimeline && !UpTimeline->IsPlaying())
		{
			UpTimeline->PlayFromStart();
		}
	}
	else
	{
		UpLocation = GetActorLocation();
		DownLocation = UpLocation - Offset;
	
		if (TranslateCurve && DownTimeline && !DownTimeline->IsPlaying())
		{
			DownTimeline->PlayFromStart();
		}
	}
	InteractCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	IsUp = !IsUp;
}

void AInteractableLiftActor::HandleTimelineProgressUp(float Value)
{
	FVector NewLocation = FMath::Lerp(DownLocation, UpLocation, Value);
	SetActorLocation(NewLocation);
}

void AInteractableLiftActor::HandleTimelineProgressDown(float Value)
{
	FVector NewLocation = FMath::Lerp(UpLocation, DownLocation, Value);
	SetActorLocation(NewLocation);
}

void AInteractableLiftActor::OnTimelineFinished()
{
	InteractCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
