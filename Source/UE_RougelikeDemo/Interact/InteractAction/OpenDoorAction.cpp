// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoorAction.h"

#include "Character/RL_BaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "System/RL_UIManagerSubsystem.h"
#include "UE_RougelikeDemo/Interact/InteractActors/InteractableDoorActor.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryComponent.h"

void UOpenDoorAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (AInteractableDoorActor* Door = Cast<AInteractableDoorActor>(InteractedTarget))
	{
		if (IsPush)
		{
			Door->PushDoor();
		}
		else
		{
			if (IsKey && !Cast<ARL_PlayerState>(Cast<ARL_BaseCharacter>(Interactor)->GetPlayerState())->
			InventoryComponent->GetItemInstanceInSlot(Cast<ARL_PlayerState>(Cast<ARL_BaseCharacter>(Interactor)->GetPlayerState())->
			InventoryComponent->GetSlotHandleByTag(KeyTag)))
			{
				URL_UIManagerSubsystem* UIManagerSubsystem = Interactor->GetGameInstance()->GetSubsystem<URL_UIManagerSubsystem>();
				UIManagerSubsystem->AddNewWidget(InteractWidgetClass, UGameplayStatics::GetPlayerController(InteractedTarget, 0));
			}
			else
			{
				Door->TranslateDoor(TranslateOffset);
			}
		}
	}
}
