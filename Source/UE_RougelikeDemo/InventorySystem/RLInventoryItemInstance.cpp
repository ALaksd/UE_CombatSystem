// Fill out your copyright notice in the Description page of Project Settings.


#include "RLInventoryItemInstance.h"
#include "RLInventoryItemDefinition.h"
#include "RLInventoryItemFragment.h"

const URLInventoryItemFragment* URLInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<URLInventoryItemFragment> FragmentClass) const
{
	// 参数深度校验
	if (!FragmentClass || !FragmentClass->IsValidLowLevel())
	{
		return nullptr;
	}

	// 静态片段查询（带有效性检查）
	if (IsValid(ItemDefinition))
	{
		const URLInventoryItemFragment* StaticFragment =
			ItemDefinition->FindFragmentByClass(FragmentClass);
		if (IsValid(StaticFragment))
		{
			return StaticFragment;
		}
	}

	// 动态片段遍历（带线程安全保护）
	TArray<URLInventoryItemFragment*> LocalDynamicFragments = DynamicFragments;
	for (URLInventoryItemFragment* Fragment : LocalDynamicFragments)
	{
		if (IsValid(Fragment))
		{
			return Fragment;
		}
	}

	return nullptr;
}


int32 URLInventoryItemInstance::GetMaxStack() const
{
	return ItemDefinition ? ItemDefinition->MaxStack : 1;
}

int32 URLInventoryItemInstance::AddStack(int32 Quantity)
{
	const int32 AvailableSpace = GetMaxStack() - CurrentStack;
	const int32 ActualAdd = FMath::Min(AvailableSpace, Quantity);
	CurrentStack += ActualAdd;
	return Quantity - ActualAdd;
}

void URLInventoryItemInstance::SetStack(int32 InitialQuantity)
{
	CurrentStack = FMath::Clamp(InitialQuantity, 1, GetMaxStack());
}

void URLInventoryItemInstance::ResetHealthBottle()
{
	CurrentStack = MaxStack;
}

void URLInventoryItemInstance::UpgradeHealthBottle()
{
	CurrentStack++;
	MaxStack++;
	MaxStack = FMath::Min(ItemDefinition->MaxStack, MaxStack);
}

void URLInventoryItemInstance::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (IsValid(ItemDefinition))
	{
		ItemDefinition->GetOwnedGameplayTags(TagContainer);
	}
}

void URLInventoryItemInstance::SetItemDefinition(URLInventoryItemDefinition* InDefinition)
{
	ItemDefinition = InDefinition;
	CurrentStack = InDefinition->InitialStack;
	MaxStack = ItemDefinition->InitialStack;
}

int32 URLInventoryItemInstance::GetInitialStack()
{
	return ItemDefinition->InitialStack;
}

FGameplayTagContainer URLInventoryItemInstance::GetOwnedGameplayTag() const
{
	if (IsValid(ItemDefinition))
	{
		return ItemDefinition->ItemTags.Added;
	}
	return FGameplayTagContainer();
}
