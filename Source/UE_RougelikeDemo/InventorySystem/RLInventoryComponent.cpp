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
}

bool URLInventoryComponent::LootItem(URLInventoryItemInstance* Item)
{
	if (!Item) return false;

	// 优先尝试堆叠到现有槽位
	for (auto& Slot : Inventory.Slots)
	{
		if (CanStackItem(Slot, Item))
		{
			const int32 Remaining = AttemptStackItem(Slot, Item);
			if (Remaining <= 0)
			{
				return true; // 堆叠成功
			}
			Item->SetStack(Remaining); // 更新剩余数量
		}
	}

	for (auto& Slot : Inventory.Slots)
	{
		FRLInventoryItemSlotHandle SlotHandle(Slot, this);
		if (AcceptsItem(Item, SlotHandle))
		{
			if (PlaceItemSlot(Item, SlotHandle))
			{
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
				return true;
			}
			
		}
	}

	// 极端情况：扩容后仍然无法放入（理论上不应该发生）
	UE_LOG(LogTemp, Warning, TEXT("Failed to place item after inventory expansion!"));
	return false;
}

bool URLInventoryComponent::LootItemByTag(URLInventoryItemInstance* Item, FGameplayTagContainer ItemTags)
{
	if (!Item) return false;

	for (auto& Slot : Inventory.Slots)
	{
		FRLInventoryItemSlotHandle SlotHandle(Slot, this);
		if (AcceptsItem(Item, SlotHandle) && Slot.SlotTags.HasAny(ItemTags))
		{
			if (PlaceItemSlot(Item, SlotHandle))
			{
				return true;
			}
		}
	}
	return false;
}


bool URLInventoryComponent::PlaceItemSlot(URLInventoryItemInstance* Item, const FRLInventoryItemSlotHandle& ItemHandle)
{
	FRLInventoryItemSlot& Slot = GetItemSlot(ItemHandle);

	//// 如果目标槽位已有物品且可堆叠
	//if (Slot.ItemInstance && CanStackItem(Slot, Item))
	//{
	//	const int32 Remaining = AttemptStackItem(Slot, Item);
	//	return Remaining == 0;
	//}

	// 原有放置逻辑（空槽或不可堆叠物品）
	if (Slot.ItemInstance != nullptr) return false;

	URLInventoryItemInstance* PreItem = Slot.ItemInstance;
	Slot.ItemInstance = Item;
	// 正确赋值SlotTags，这里暂时不会用CombindTags
	if (Item->GetItemDefinition())
	{
		Slot.SlotTags = Item->GetItemDefinition()->ItemTags.Added;
	}
	else
	{
		Slot.SlotTags.Reset(); // 没有ItemDefinition则清空
	}



	// 更新对应的句柄标签
	for (auto& Handle : AllSlotHandles)
	{
		if (Handle.SlotId == Slot.SlotId)
		{
			Handle.SlotTags = Slot.SlotTags;
			break;
		}
	}


	// 广播更新事件
	OnItemSlotUpdate.Broadcast(this, ItemHandle, Slot.ItemInstance, PreItem);
	return true;
}

bool URLInventoryComponent::RemoveItemFromInventory(const FRLInventoryItemSlotHandle& SlotHandle, int32 RemoveQuantity)
{
	FRLInventoryItemSlot& ItemSlot = GetItemSlot(SlotHandle);
	if (!ItemSlot.ItemInstance) return false;

	// 处理堆叠物品
	if (ItemSlot.ItemInstance->GetItemDefinition()->IsStackable())
	{
		const int32 NewQuantity = ItemSlot.ItemInstance->GetCurrentStack() - RemoveQuantity;
		if (NewQuantity > 0)
		{
			ItemSlot.ItemInstance->SetStack(NewQuantity);
			OnItemSlotUpdate.Broadcast(this, SlotHandle, ItemSlot.ItemInstance, ItemSlot.ItemInstance);
			return true;
		}
	}

	// 更新对应的句柄标签
	for (auto& Handle : AllSlotHandles)
	{
		if (Handle.SlotId == ItemSlot.SlotId)
		{
			Handle.SlotTags.Reset(); // 清空标签
			break;
		}
	}

	//移除不可堆叠物品
	URLInventoryItemInstance* PreviousItem = ItemSlot.ItemInstance;
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

TArray<FRLInventoryItemSlotHandle> URLInventoryComponent::GetSlotHandlesByTags(const FGameplayTagContainer& Tags)
{
	TArray<FRLInventoryItemSlotHandle> MatchingHandles;
	for (const FRLInventoryItemSlot& Slot : Inventory.Slots)
	{
		if (Slot.ItemInstance && Slot.SlotTags.HasAny(Tags))
		{
			MatchingHandles.Add(FRLInventoryItemSlotHandle(Slot, this));
		}
	}
	return MatchingHandles;
}

FRLInventoryItemSlotHandle URLInventoryComponent::GetSlotHandleByTag(const FGameplayTag& Tag)
{
	for (const FRLInventoryItemSlot& Slot : Inventory.Slots)
	{
		if (Slot.ItemInstance && Slot.SlotTags.HasTagExact(Tag))
		{
			return FRLInventoryItemSlotHandle(Slot, this);
		}
	}
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

bool URLInventoryComponent::CanStackItem(const FRLInventoryItemSlot& Slot, URLInventoryItemInstance* NewItem) const
{
	if (!Slot.ItemInstance)
		return false;
	bool bSame = Slot.ItemInstance->GetItemDefinition() == NewItem->GetItemDefinition();
	bool bStackable = Slot.ItemInstance->GetItemDefinition()->IsStackable() && Slot.ItemInstance->GetCurrentStack() < Slot.ItemInstance->GetMaxStack();

	return bSame && bStackable;
}

int32 URLInventoryComponent::AttemptStackItem(FRLInventoryItemSlot& Slot, URLInventoryItemInstance* NewItem)
{
	URLInventoryItemInstance* ExistingItem = Slot.ItemInstance;
	const int32 AvailableSpace = ExistingItem->GetMaxStack() - ExistingItem->GetCurrentStack();
	const int32 ToAdd = FMath::Min(AvailableSpace, NewItem->GetCurrentStack());

	const int32 Remaining = ExistingItem->AddStack(ToAdd);

	// 通知堆叠数量变化
	OnItemSlotUpdate.Broadcast(this, FRLInventoryItemSlotHandle(Slot, this), ExistingItem, ExistingItem);

	return Remaining;
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

	PostInventoryUpdate();
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
