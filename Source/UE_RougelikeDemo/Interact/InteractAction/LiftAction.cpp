// Fill out your copyright notice in the Description page of Project Settings.


#include "LiftAction.h"

#include "GameFramework/Character.h"
#include "UE_RougelikeDemo/Interact/InteractActors/InteractableLiftActor.h"

void ULiftAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (ACharacter* Character = Cast<ACharacter>(Interactor))
	{
		Cast<AInteractableLiftActor>(InteractedTarget)->LiftRunning(Offset);
	}
}
