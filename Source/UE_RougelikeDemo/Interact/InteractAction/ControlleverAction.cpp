// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlleverAction.h"

#include "Character/RL_BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UE_RougelikeDemo/Interact/InteractActors/InteractableControlleverActor.h"
#include "UE_RougelikeDemo/Interact/InteractActors/InteractableLiftActor.h"

void UControlleverAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (AInteractableControlleverActor* Controllever = Cast<AInteractableControlleverActor>(InteractedTarget))
	{
		if (Controllever->IsPush)
		{
			Controllever->LeverMesh->PlayAnimation(Controllever->PushAnim, false);
		}
		else
		{
			Controllever->LeverMesh->PlayAnimation(Controllever->PullAnim, false);
		}
		Controllever->InteractCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Controllever->IsPush = !Controllever->IsPush;
		IInteractableInterface::Execute_Interact(Controllever->Lift, Interactor);
	}
}
