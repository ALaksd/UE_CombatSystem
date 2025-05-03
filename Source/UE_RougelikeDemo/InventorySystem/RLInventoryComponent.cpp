// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_RougelikeDemo\InventorySystem\RLInventoryComponent.h"
#include "RLInventoryItemInstance.h"
#include "RLInventoryItemDefinition.h"
#include "Kismet/GameplayStatics.h"
#include "MessagSystem/MessageSubsystem.h"
#include "MessagSystem/Message_GetItem.h"

URLInventoryComponent::URLInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void URLInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	//CreateInventorySlot(20);
}

bool URLInventoryComponent::LootItem(URLInventoryItemInstance* Item)
{
	if (!Item) return false;

	// 第一次尝试放入
	for (auto& Slot : Inventory.Slots)
	{
		FRLInventoryItemSlotHandle SlotHandle(Slot, this);
		if (AcceptsItem(Item, SlotHandle))
		{
			if (PlaceItemSlot(Item, SlotHandle))
			{
				//向UI广播放入事件
				//OnItemSlotUpdate.Broadcast(this,SlotHandle,Item,nullptr);
				return true;
			}
		}
	}

	// 如果所有格子已满则自动扩容
	const int CurrentCapacity = Inventory.Slots.Num();
	const int GrowthAmount = FMath::Max(
		static_cast<int>(CurrentCapacity * CapacityGrowthFactor),
		MinGrowthAmount
	);

	CreateInventorySlot(GrowthAmount);

	// 尝试放入新扩容的格子（只需检查新增的格子）
	for (int32 i = CurrentCapacity; i < Inventory.Slots.Num(); ++i)
	{
		FRLInventoryItemSlotHandle SlotHandle(Inventory.Slots[i], this);
		if (AcceptsItem(Item, SlotHandle))
		{
			if (PlaceItemSlot(Item, SlotHandle))
			{
				//向UI广播放入事件
				//OnItemSlotUpdate.Broadcast(this,SlotHandle,Item,nullptr);
				return true;
			}
		}
	}

	// 极端情况：扩容后仍然无法放入（理论上不应该发生）
	UE_LOG(LogTemp, Warning, TEXT("Failed to place item after inventory expansion!"));
	return false;
}


bool URLInventoryComponent::PlaceItemSlot(URLInventoryItemInstance* Item, const FRLInventoryItemSlotHandle& ItemHandle)
{
	if (!Item) return false;

	FRLInventoryItemSlot& Slot = GetItemSlot(ItemHandle);
	URLInventoryItemInstance* PreItem = Slot.ItemInstance;	
	Slot.ItemInstance = Item;
	// 正确赋值SlotTags
	if (Item->GetItemDefinition())
	{
		Slot.SlotTags = Item->GetItemDefinition()->ItemTags.CombinedTags;
	}
	else
	{
		Slot.SlotTags.Reset(); // 没有ItemDefinition则清空
	}

	OnItemSlotUpdate.Broadcast(this,ItemHandle,Slot.ItemInstance,PreItem);

	return true;
}

bool URLInventoryComponent::RemoveItemFromInventory(const FRLInventoryItemSlotHandle& SlotHandle)
{
	FRLInventoryItemSlot& ItemSlot = GetItemSlot(SlotHandle);
	URLInventoryItemInstance* PreviousItem = ItemSlot.ItemInstance;

	if (!ItemSlot.ItemInstance) return false;

	ItemSlot.ItemInstance = nullptr;
	ItemSlot.SlotTags.Reset();

	OnItemSlotUpdate.Broadcast(this, SlotHandle, ItemSlot.ItemInstance, PreviousItem);

	return true;
}

bool URLInventoryComponent::RemoveAllItemsFromInventory(TArray<URLInventoryItemInstance*>& OutItemsRemoved)
{
	for (FRLInventoryItemSlot& ItemSlot : Inventory.Slots)
	{
		if (!ItemSlot.ItemInstance) continue;

		OutItemsRemoved.Add(ItemSlot.ItemInstance);
		RemoveItemFromInventory(FRLInventoryItemSlotHandle(ItemSlot, this));
	}
	return true;
}

TArray<FRLInventoryItemSlotHandle> URLInventoryComponent::GetAllSlotHandles()
{
	return AllSlotHandles;
}

FRLInventoryItemSlotHandle URLInventoryComponent::GetSlotHandleByTags(FGameplayTagContainer Tags)
{
	for (const FRLInventoryItemSlot& Slot : Inventory.Slots)
	{
		if (Slot.ItemInstance && Slot.SlotTags.HasAll(Tags))
		{
			return FRLInventoryItemSlotHandle(Slot, this);
		}
	}

	// 没找到返回空Handle（SlotId = -1）
	return FRLInventoryItemSlotHandle();
}

URLInventoryItemInstance* URLInventoryComponent::GetItemInstanceInSlot(const FRLInventoryItemSlotHandle& Handle)
{
	if (!IsVaildItemSlot(Handle))
		return nullptr;

	FRLInventoryItemSlot& Slot = GetItemSlot(Handle);
	return Slot.ItemInstance;
}

bool URLInventoryComponent::IsVaildItemSlot(const FRLInventoryItemSlotHandle& Handle)
{
	for (const auto& Item : AllSlotHandles)
	{
		if (Item == Handle)
		{
			return true;
		}
	}
	return false;
}

FRLInventoryItemSlot& URLInventoryComponent::GetItemSlot(const FRLInventoryItemSlotHandle& Handle)
{
	if (IsVaildItemSlot(Handle))
	{
		for (auto& Slot : Inventory.Slots)
		{
			if (Slot.SlotId == Handle.SlotId)
			{
				return Slot;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Handle is not Vaild"));

	return Inventory.Slots[0];
}

bool URLInventoryComponent::AcceptsItem(URLInventoryItemInstance* Item, const FRLInventoryItemSlotHandle& ItemHandle)
{
	if (GetItemSlot(ItemHandle).ItemInstance == nullptr && GetItemSlot(ItemHandle).ItemInstance != Item)
	{
		return true;
	}
	return false;
}

/*
 * 消息系统
 */
// void URLInventoryComponent::SendGetItemMessage(URLInventoryItemInstance* Item, const FRLInventoryItemSlotHandle& Handle)
// {
// 	if (!MsgSystem)
// 	{
// 		if (UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld()))
// 		{
// 			MsgSystem = GI->GetSubsystem<UMessageSubsystem>();
// 		}
// 	}
//
// 	if (MsgSystem)
// 	{
// 		UMessage_GetItem* ItemMsg = NewObject<UMessage_GetItem>(this);
// 		ItemMsg->ItemInstance=Item;
// 		ItemMsg->SlotHandle=Handle;
// 		MsgSystem->Publish(ItemMsg);
// 	}
// }

void URLInventoryComponent::CreateInventorySlot(int32 Count)
{
	for (int32 i = 0; i < Count; i++)
	{
		FRLInventoryItemSlot NewSlot;
		NewSlot.ItemInstance = nullptr;
		NewSlot.SlotId = IdCounter;
		NewSlot.Onwer = this;

		IdCounter++;
		Inventory.Slots.Add(NewSlot);
	}


	PostInventoryUpdate();
}

void URLInventoryComponent::CreateInventorySlotByTag(FGameplayTag Tag)
{
	//根据Tag生成背包插槽，不用重新生成
	FRLInventoryItemSlot NewSlot;
	NewSlot.ItemInstance = nullptr;
	NewSlot.SlotId = IdCounter;
	NewSlot.Onwer = this;
	NewSlot.SlotTags.AddTag(Tag);

	IdCounter++;
	Inventory.Slots.Add(NewSlot);
}

void URLInventoryComponent::RemoveInventorySlot(const FRLInventoryItemSlotHandle& Handle)
{
	if (!IsVaildItemSlot(Handle)) return;

	FRLInventoryItemSlot& RemovedSlot = GetItemSlot(Handle);
	Inventory.Slots.Remove(RemovedSlot);

	PostInventoryUpdate();
}

void URLInventoryComponent::PostInventoryUpdate()
{
	//首先清空存放钥匙的数组，然后重新生成
	AllSlotHandles.Empty(AllSlotHandles.Num() + 1);
	PopulateSlotReferenceArray(AllSlotHandles);
}

void URLInventoryComponent::PopulateSlotReferenceArray(TArray<FRLInventoryItemSlotHandle>& Handles)
{
	//按照当前的插槽重新生成钥匙数组
	for (int i = 0; i < Inventory.Slots.Num(); i++)
	{
		FRLInventoryItemSlotHandle Handle(Inventory.Slots[i], this);
		AllSlotHandles.Add(Handle);
	}
}
