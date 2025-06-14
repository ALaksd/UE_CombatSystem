// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplaceActorAction.h"

#include "UE_RougelikeDemo/Interact/InteractableActor.h"

void UReplaceActorAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (Interactor->GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AActor* Actor = Interactor->GetWorld()->SpawnActor<AActor>(
			NewActor,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);

		InteractedTarget->Destroy();
	}
}
