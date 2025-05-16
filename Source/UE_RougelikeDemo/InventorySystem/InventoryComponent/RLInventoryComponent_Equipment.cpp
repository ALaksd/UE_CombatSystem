// Fill out your copyright notice in the Description page of Project Settings.


#include "RLInventoryComponent_Equipment.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemDefinition.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLInventoryFragment_Equipment.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemInstance.h"

URLInventoryComponent_Equipment::URLInventoryComponent_Equipment(const FObjectInitializer& ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

void URLInventoryComponent_Equipment::InitializeComponent()
{
	Super::InitializeComponent();

	// 动态创建所有预设槽位
	for (const FEquipmentSlotGroup& Group : SlotGroups)
	{
		for (int32 i = 0; i < Group.SlotCount; ++i)
		{
			// 创建带编号的Tag（示例：Item.Weapon.Slot.1）
			FGameplayTag NumberedTag = FGameplayTag::RequestGameplayTag(FName(Group.SlotTypeTag.GetTagName().ToString().Append(FString::Printf(TEXT(".%d"), i + 1))));

			//创建插槽
			CreateInventorySlotByTag(NumberedTag);
			// 创建槽位Handle
			FRLInventoryItemSlotHandle NewHandle(NumberedTag,this);
		}
	}
}


bool URLInventoryComponent_Equipment::PlaceItemSlot(URLInventoryItemInstance* Item, const FRLInventoryItemSlotHandle& ItemHandle)
{
	if (!Item) return false;

	FRLInventoryItemSlot& Slot = GetItemSlot(ItemHandle);
	URLInventoryItemInstance* PreItem = Slot.ItemInstance;
	Slot.ItemInstance = Item;
	OnItemSlotUpdate.Broadcast(this, ItemHandle, Slot.ItemInstance, PreItem);

	return true;
}

bool URLInventoryComponent_Equipment::RemoveItemFromInventory(const FRLInventoryItemSlotHandle& SlotHandle, int32 RemoveQuantity)
{
	FRLInventoryItemSlot& ItemSlot = GetItemSlot(SlotHandle);
	URLInventoryItemInstance* PreviousItem = ItemSlot.ItemInstance;

	if (!ItemSlot.ItemInstance) return false;

	ItemSlot.ItemInstance = nullptr;

	OnItemSlotUpdate.Broadcast(this, SlotHandle, ItemSlot.ItemInstance, PreviousItem);

	return true;
}

URLInventoryItemInstance* URLInventoryComponent_Equipment::GetEqeippedItemByType(FGameplayTag SlotTypeTag)
{
	for (const FRLInventoryItemInfoEntry& Entry : EquipmentInfos)
	{
		if (Entry.Handle.SlotTags.HasTagExact(SlotTypeTag))
		{
			if (URLInventoryItemInstance* Item = GetItemInstanceInSlot(Entry.Handle))
			{
				return Item;
			}
		}
	}

	return nullptr;
}

void URLInventoryComponent_Equipment::SwitchWeapon(const FInputActionValue& Value)
{
	// 如果只装备一把武器,不进行操作
	if (Weapon1.ItemInstance == nullptr || Weapon2.ItemInstance == nullptr) return;
	
	// 移除手上武器GA
	// 给予另一把武器GA
	if (CurrentWeapon==Weapon1)
	{
		RemoveAbilityFromPlayer(Weapon1);
		GiveAbilityToPlayer(Weapon2);
		CurrentWeapon=Weapon2;
	}
	else
	{
		RemoveAbilityFromPlayer(Weapon2);
		GiveAbilityToPlayer(Weapon1);
		CurrentWeapon=Weapon1;
	}

	// 通知ClosecombatComponent
	OnEquipUpdate.Broadcast(CurrentWeapon.ItemInstance);
}

void URLInventoryComponent_Equipment::BeginPlay()
{
	Super::BeginPlay();

	OnItemSlotUpdate.AddDynamic(this, &URLInventoryComponent_Equipment::OnEquipSlotUpdate);

}

void URLInventoryComponent_Equipment::OnEquipSlotUpdate(URLInventoryComponent* InventoryComponent, const FRLInventoryItemSlotHandle& SlotHandle, URLInventoryItemInstance* ItemInstance, URLInventoryItemInstance* PreviousItemInstance)
{
	/*
	* 首先脱下前一个装备；
	* 当前装备等于前一个装备，返回；
	* 否则穿上；
	*/
	if (IsValid(PreviousItemInstance))
	{
		// 如果将要装备的物品是武器则调用武器专有装备函数
		if (SlotHandle.SlotTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Item.Weapon.Slot.1"))) || SlotHandle.SlotTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Item.Weapon.Slot.2"))))
			UnEquipWeapon(SlotHandle, PreviousItemInstance);
		else
			MakeItemUnequipped_Internal(SlotHandle, PreviousItemInstance);
	}
	if (IsValid(ItemInstance))
	{
		if (ItemInstance != PreviousItemInstance)
		{
			// 如果将要装备的物品是武器则调用武器专有装备函数
			if (SlotHandle.SlotTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Item.Weapon.Slot.1"))) || SlotHandle.SlotTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Item.Weapon.Slot.2"))))
				EquipWeapon(SlotHandle, ItemInstance);
			else
				MakeItemEquipped_Internal(SlotHandle, ItemInstance);
		}

	}
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

	ItemInstance->SetbEquiped(true);
	bOnEquip.ExecuteIfBound(true);
	OnEquipUpdate.Broadcast(CurrentWeapon.ItemInstance);
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

	// 更新物品实例的装备状态
	ItemInstance->SetbEquiped(false);
	bOnEquip.ExecuteIfBound(false);
	OnEquipUpdate.Broadcast(CurrentWeapon.ItemInstance);
	return true;
}

void URLInventoryComponent_Equipment::EquipWeapon(const FRLInventoryItemSlotHandle& SlotHandle,URLInventoryItemInstance* ItemInstance)
{
	// 新的武器tag为武器1
	if (SlotHandle.SlotTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Item.Weapon.Slot.1"))))
	{
		Weapon1.SlotTag = SlotHandle.SlotTags.GetByIndex(0);
		Weapon1.Handle=SlotHandle;
		Weapon1.ItemInstance = ItemInstance;
	}
	else
	{
		Weapon2.SlotTag = SlotHandle.SlotTags.GetByIndex(0);
		Weapon2.Handle=SlotHandle;
		Weapon2.ItemInstance = ItemInstance;
	}

	if (CurrentWeapon.ItemInstance == nullptr)
	{
		// 目前武器只有一个Tag,暂时这么写
		if (SlotHandle.SlotTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("Item.Weapon.Slot.1"))))
		{
			GiveAbilityToPlayer(Weapon1);
			CurrentWeapon=Weapon1;
		}
		else
		{
			GiveAbilityToPlayer(Weapon2);
			CurrentWeapon=Weapon2;
		}
		
		OnEquipUpdate.Broadcast(CurrentWeapon.ItemInstance);
	}
	FRLInventoryItemInfoEntry Entry(SlotHandle);
	EquipmentInfos.Add(Entry);

	// 更新物品实例的装备状态
	ItemInstance->SetbEquiped(true);
	bOnEquip.ExecuteIfBound(true);
}

void URLInventoryComponent_Equipment::UnEquipWeapon(const FRLInventoryItemSlotHandle& SlotHandle,URLInventoryItemInstance* ItemInstance)
{
	// 如果要卸的武器为当前装备的武器
	/*
	 * 移除GA
	 * 从装备列表移除
	 */
	if (!IsValid(ItemInstance)) return;

	if (CurrentWeapon.ItemInstance==ItemInstance)
	{
		if (UASC_Base* ASC = Cast<UASC_Base>(GetOwnerAbilitySystemComponent()))
		{
			CurrentWeapon.AbilityGrantHandles.TakeFromAbilitySystem(ASC);

			CurrentWeapon.ItemInstance=nullptr;
		}
	}

	// 移除条目
	EquipmentInfos.RemoveAllSwap(
		[SlotHandle](const FRLInventoryItemInfoEntry& InfoEntry) {
			return InfoEntry.Handle == SlotHandle;
		},
		EAllowShrinking::No);

	// 更新物品实例的装备状态
	OnEquipUpdate.Broadcast(nullptr);
	ItemInstance->SetbEquiped(false);
	bOnEquip.ExecuteIfBound(false);
}

void URLInventoryComponent_Equipment::GiveAbilityToPlayer(FEquipWeapon& Weapon)
{
	if (const URLInventoryFragment_Equipment* EquipmentFragment = Weapon.ItemInstance->FindFragmentByClass<URLInventoryFragment_Equipment>())
	{
		if (UAbilitySystemComponent* ASC = GetOwnerAbilitySystemComponent())
		{
			FRLInventoryItemInfoEntry Entry(Weapon.Handle);
			EquipmentFragment->GiveToAbilitySystem(ASC, &Weapon.AbilityGrantHandles, Weapon.ItemInstance);
		}
	}
}

void URLInventoryComponent_Equipment::RemoveAbilityFromPlayer(FEquipWeapon& Weapon)
{
	// 移除能力
	if (UASC_Base* ASC = Cast<UASC_Base>(GetOwnerAbilitySystemComponent()))
	{
		Weapon.AbilityGrantHandles.TakeFromAbilitySystem(ASC);
	}	
}


UAbilitySystemComponent* URLInventoryComponent_Equipment::GetOwnerAbilitySystemComponent()
{
	//这里需要把这个组件放在playerSyate上
	UAbilitySystemComponent* ASC = GetOwner()->GetComponentByClass<UAbilitySystemComponent>();
	return ASC;
}
