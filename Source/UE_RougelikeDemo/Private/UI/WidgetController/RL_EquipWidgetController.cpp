// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/RL_EquipWidgetController.h"
#include "GameFramework/PlayerState.h"
#include "UE_RougelikeDemo/InventorySystem/InventoryComponent/RLInventoryComponent_Equipment.h"

void URL_EquipWidgetController::BroadcastInitialValue()
{
}

void URL_EquipWidgetController::BindCallbacksToDependencies()
{
	EquipmentInventory = PlayerState->FindComponentByClass<URLInventoryComponent_Equipment>();
	//if (EquipmentInventory)
	//{
	//	EquipmentInventory->OnEquipUpdate.AddDynamic(this, &URL_EquipWidgetController::HandleEquipUpdate);
	//}
}

void URL_EquipWidgetController::UnbindAllDelegate()
{
	//OnEquipItemSlotUpdate.RemoveDynamic(this, &URL_EquipWidgetController::HandleEquipUpdate);
}

void URL_EquipWidgetController::HandleEquipUpdate(URLInventoryComponent* InventoryComponent, const FRLInventoryItemSlotHandle& SlotHandle,URLInventoryItemInstance* NewItem, URLInventoryItemInstance* OldItem)
{
	//OnEquipItemSlotUpdate.Broadcast(InventoryComponent, SlotHandle,NewItem, OldItem);
}
