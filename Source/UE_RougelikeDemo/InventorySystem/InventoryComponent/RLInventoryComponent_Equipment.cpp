// Fill out your copyright notice in the Description page of Project Settings.


#include "RLInventoryComponent_Equipment.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemDefinition.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLInventoryFragment_Equipment.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemInstance.h"

URLInventoryComponent_Equipment::URLInventoryComponent_Equipment(const FObjectInitializer& ObjectInitializer)
{
	
}

void URLInventoryComponent_Equipment::InitializeComponent()
{
	Super::InitializeComponent();

}

void URLInventoryComponent_Equipment::BeginPlay()
{
	Super::BeginPlay();

	OnItemSlotUpdate.AddDynamic(this, &URLInventoryComponent_Equipment::OnEquipSlotUpdate);

}

void URLInventoryComponent_Equipment::OnEquipSlotUpdate(URLInventoryComponent* InventoryComponent, const FRLInventoryItemSlotHandle& SlotHandle, URLInventoryItemInstance* ItemInstance, URLInventoryItemInstance* PreviousItemInstance)
{
	//前一个装备不为空，则首先将前一个装备脱下；如果当前装备不为空，则将当前装备穿上。
	if (IsValid(PreviousItemInstance))
	{
		MakeItemUnequipped_Internal(SlotHandle, PreviousItemInstance);
	}
	if (IsValid(ItemInstance))
	{
		MakeItemEquipped_Internal(SlotHandle, ItemInstance);
	}
	OnEquipUpdate.Broadcast(ItemInstance, PreviousItemInstance);
}

bool URLInventoryComponent_Equipment::MakeItemEquipped_Internal(const FRLInventoryItemSlotHandle& SlotHandle, URLInventoryItemInstance* ItemInstance)
{
	/**
	 * 1.从Item Instance中获取Item Definition
	 * 2.获取装备功能相关的Equipment Fragment
	 * 3.调用Equipment Fragment中实现的函数，将GE和GA添加给角色
	 * 4.将handles储存起来
	 */
	if (!IsValid(ItemInstance)) return false;

	URLInventoryItemDefinition* ItemDefinition = ItemInstance->GetItemDefinition();
	if (!IsValid(ItemDefinition)) return false;
	
	if (const URLInventoryFragment_Equipment* EquipmentFragment = ItemInstance->FindFragmentByClass<URLInventoryFragment_Equipment>())
	{
		if (UAbilitySystemComponent* ASC = GetOwnerAbilitySystemComponent())
		{
			FRLInventoryItemInfoEntry Entry(SlotHandle);
			EquipmentFragment->GiveToAbilitySystem(ASC, &Entry.AbilityGrantHandles, ItemInstance);
			EquipmentInfos.Add(Entry);
		}
	}
	return true;
}


bool URLInventoryComponent_Equipment::MakeItemUnequipped_Internal(const FRLInventoryItemSlotHandle& SlotHandle, URLInventoryItemInstance* ItemInstance)
{
	/**
	 * 1.从EquipmentInfos中找到指定物品的效果容器
	 * 2.通过EquipmentInfo中储存的GA handles和GEhandles，将对应的效果从角色身上移除
	 * 3.将equipmentinfo移除
	 */

	if (!IsValid(ItemInstance)) return false;

	// 查找装备条目
	FRLInventoryItemInfoEntry* Entry = EquipmentInfos.FindByPredicate(
		[SlotHandle](const FRLInventoryItemInfoEntry& InfoEntry) {
			return InfoEntry.Handle == SlotHandle;
		});

	if (!Entry) return false;

	// 移除能力
	if (UASC_Base* ASC = Cast<UASC_Base>(GetOwnerAbilitySystemComponent()))
	{
		Entry->AbilityGrantHandles.TakeFromAbilitySystem(ASC);
	}	

	// 移除条目
	EquipmentInfos.RemoveAllSwap(
		[SlotHandle](const FRLInventoryItemInfoEntry& InfoEntry) {
			return InfoEntry.Handle == SlotHandle;
		},
		EAllowShrinking::No);

	return true;
}

UAbilitySystemComponent* URLInventoryComponent_Equipment::GetOwnerAbilitySystemComponent()
{
	//这里需要把这个组件放在playerSyate上
	UAbilitySystemComponent* ASC = GetOwner()->GetComponentByClass<UAbilitySystemComponent>();
	return ASC;
}
