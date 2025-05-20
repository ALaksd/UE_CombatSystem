// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/RL_InventoryWidgetController.h"
#include "GameFramework/PlayerState.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryComponent.h"
#include <UE_RougelikeDemo/InventorySystem/InventoryComponent/RLInventoryComponent_Equipment.h>

void URL_InventoryWidgetController::BroadcastInitialValue()
{

}

void URL_InventoryWidgetController::BindCallbacksToDependencies()
{
	InventoryComponnet = PlayerState->FindComponentByClass<URLInventoryComponent>();
	URLInventoryComponent_Equipment* InventoryComponent_Equipment = PlayerState->FindComponentByClass<URLInventoryComponent_Equipment>();
	if (InventoryComponnet)
	{
		InventoryComponnet->OnItemSlotUpdate.AddDynamic(this, &URL_InventoryWidgetController::HandleItemSlotUpdate);
	}
	if (InventoryComponent_Equipment)
	{
		InventoryComponent_Equipment->bOnEquip.BindUObject(this, &URL_InventoryWidgetController::SetbEquiped);

	}
}

void URL_InventoryWidgetController::UnbindAllDelegate()
{
	OnInventoryItemSlotUpdate.RemoveDynamic(this, &URL_InventoryWidgetController::HandleItemSlotUpdate);
	bOnEquip_UI.Clear();
}

void URL_InventoryWidgetController::HandleItemSlotUpdate(URLInventoryComponent* InventoryComponent, const FRLInventoryItemSlotHandle& SlotHandle, URLInventoryItemInstance* NewItem, URLInventoryItemInstance* OldItem)
{
	OnInventoryItemSlotUpdate.Broadcast(InventoryComponent, SlotHandle, NewItem, OldItem);
}

void URL_InventoryWidgetController::SetbEquiped(bool bEquip)
{
	bOnEquip_UI.Broadcast(bEquip);
}
