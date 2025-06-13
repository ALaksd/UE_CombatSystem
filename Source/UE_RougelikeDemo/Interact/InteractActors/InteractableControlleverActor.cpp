// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableControlleverActor.h"

#include "InteractableLiftActor.h"
#include "Components/BoxComponent.h"
#include "UE_RougelikeDemo/Interact/InteractAction/LiftAction.h"

AInteractableControlleverActor::AInteractableControlleverActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	LeverMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LiftMesh"));
	LeverMesh->SetupAttachment(InteractCollision);
}

void AInteractableControlleverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Lift->IsUp == IsUp)
	{
		InteractCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
}
