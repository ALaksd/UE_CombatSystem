// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/RL_InventoryComponent.h"

URL_InventoryComponent::URL_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void URL_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URL_InventoryComponent::AddItem(URL_ItemDataAsset* ItemData, int32 Count)
{
	if (!ItemData) return;

	EItemType Type = ItemData->ItemType;
	FName ID = ItemData->ItemID;

	if (ItemData->bCanStack)
	{
		// 消耗品类，可叠加，增加堆叠数量
		if (UseItems.Contains(ID))
		{
			UseItems[ID].Count += Count;
		}
		else
		{
			FInventorySlot NewSlot;
			NewSlot.ItemID = ID;
			NewSlot.ItemData = ItemData;
			NewSlot.Count = Count;
			UseItems.Add(ID, NewSlot);
		}
	}
	else
	{
		// 装备/武器类，不能叠加，每件都是一项
		FInventorySlot NewSlot;
		NewSlot.ItemID = ID;
		NewSlot.ItemData = ItemData;
		NewSlot.Count = 1;
		NewSlot.UniqueID = FGuid::NewGuid(); // 生成唯一 ID

		if (Type == EItemType::Weapon)
			WeaponItems.Add(NewSlot);
		else if (Type == EItemType::Equipment)
			EquipmentItems.Add(NewSlot);
	}
}


