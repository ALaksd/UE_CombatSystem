// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableNPCActor.h"

#include "EnhancedInputSubsystems.h"
#include "Components/BoxComponent.h"

AInteractableNPCActor::AInteractableNPCActor()
{
	PrimaryActorTick.bCanEverTick = false;

	NPCMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NPCMesh"));
	NPCMesh->SetupAttachment(InteractCollision);
}

void AInteractableNPCActor::BeginPlay()
{
	Super::BeginPlay();
	Index = 0;
}

void AInteractableNPCActor::BanAct(APlayerController* PlayerController)
{
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (InputSubsystem && BaseIMC && MoveIMC)
	{
		InputSubsystem->RemoveMappingContext(BaseIMC);
		InputSubsystem->RemoveMappingContext(MoveIMC);
	}
}

void AInteractableNPCActor::FreeAct(APlayerController* PlayerController)
{
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (InputSubsystem && BaseIMC && MoveIMC)
	{
		InputSubsystem->AddMappingContext(BaseIMC, 0);
		InputSubsystem->AddMappingContext(MoveIMC, 1);
	}
}
