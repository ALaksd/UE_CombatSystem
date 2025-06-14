// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupAction.h"

#include "Item/Item_Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RL_PlayerState.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryComponent.h"

void UPickupAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (InteractedTarget)
	{
		// 物品加入背包
		ARL_PlayerState* PlayerState = Cast<ARL_PlayerState>(UGameplayStatics::GetPlayerState(Interactor->GetWorld(),0));

		//这里先暂时加入装备背包
		URLInventoryComponent* BackpComponent = Cast<URLInventoryComponent>(PlayerState->FindComponentByClass(URLInventoryComponent::StaticClass()));

		AItem_Pickup* Item = Cast<AItem_Pickup>(InteractedTarget);
		
		if (BackpComponent)
		{
			BackpComponent->LootItem(Item->ItemInstance);
			// 销毁地上的物品
			Item->Destroy();
		}
	}
}
