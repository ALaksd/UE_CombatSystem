// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/RL_InventoryWidgetController.h"
#include "GameFramework/PlayerState.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryComponent.h"

void URL_InventoryWidgetController::BroadcastInitialValue()
{

}

void URL_InventoryWidgetController::BindCallbacksToDependencies()
{
	InventoryComponnet = PlayerState->FindComponentByClass<URLInventoryComponent>();
	if (InventoryComponnet)
	{
		InventoryComponnet->OnItemSlotUpdate.AddDynamic(this, &URL_InventoryWidgetController::HandleItemSlotUpdate);
	}
}

void URL_InventoryWidgetController::UnbindAllDelegate()
{
	OnInventoryItemSlotUpdate.RemoveDynamic(this, &URL_InventoryWidgetController::HandleItemSlotUpdate);
}

void URL_InventoryWidgetController::HandleItemSlotUpdate(URLInventoryComponent* InventoryComponent, const FRLInventoryItemSlotHandle& SlotHandle, URLInventoryItemInstance* NewItem, URLInventoryItemInstance* OldItem)
{
	OnInventoryItemSlotUpdate.Broadcast(InventoryComponent, SlotHandle, NewItem, OldItem);
}
