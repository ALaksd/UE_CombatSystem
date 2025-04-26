// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/RL_EquipWidgetController.h"
#include "GameFramework/PlayerState.h"
#include "UE_RougelikeDemo/InventorySystem/InventoryComponent/RLInventoryComponent_Equipment.h"

void URL_EquipWidgetController::BroadcastInitialValue()
{
}

void URL_EquipWidgetController::BindCallbacksToDependencies()
{
	URLInventoryComponent_Equipment* EquipmentInventoryComponnet = PlayerState->FindComponentByClass<URLInventoryComponent_Equipment>();
	if (EquipmentInventoryComponnet)
	{
		EquipmentInventoryComponnet->OnEquipUpdate.AddDynamic(this, &URL_EquipWidgetController::HandleEquipUpdate);
	}
}

void URL_EquipWidgetController::UnbindAllDelegate()
{
	OnEquipItemSlotUpdate.RemoveDynamic(this, &URL_EquipWidgetController::HandleEquipUpdate);
}

void URL_EquipWidgetController::HandleEquipUpdate(URLInventoryItemInstance* NewItem, URLInventoryItemInstance* OldItem)
{
	OnEquipItemSlotUpdate.Broadcast(NewItem, OldItem);
}
