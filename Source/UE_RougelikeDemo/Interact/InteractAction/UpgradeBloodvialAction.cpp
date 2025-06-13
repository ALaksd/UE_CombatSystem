// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeBloodvialAction.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemInstance.h"
#include "UE_RougelikeDemo/InventorySystem/InventoryComponent/RLInventoryComponent_Equipment.h"

void UUpgradeBloodvialAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	URLInventoryComponent* InventoryComp = Cast<ACharacter>(Interactor)->GetPlayerState()->FindComponentByClass<URLInventoryComponent>();
	if (InventoryComp)
	{
		FRLInventoryItemSlotHandle InventorySlotHandle = InventoryComp->GetSlotHandleByTag(FGameplayTag::RequestGameplayTag("Item.Use.HealthPotion"));
		URLInventoryItemInstance* BottleInstance = InventoryComp->GetItemInstanceInSlot(InventorySlotHandle);
		if (BottleInstance)
		{
			BottleInstance->UpgradeHealthBottle();
			InventoryComp->OnItemSlotUpdate.Broadcast(InventoryComp, InventorySlotHandle, BottleInstance, BottleInstance);

		}
	}
	InteractedTarget->Destroy();
}
