// Fill out your copyright notice in the Description page of Project Settings.


#include "RLInventoryItemInstance.h"
#include "RLInventoryItemDefinition.h"
#include "RLInventoryItemFragment.h"

const URLInventoryItemFragment* URLInventoryItemInstance::FindFragmentByClass(TSubclassOf<URLInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		if (ItemDefinition != nullptr && ItemDefinition->FindFragmentByClass(FragmentClass))
		{
			return ItemDefinition->FindFragmentByClass(FragmentClass);
		}
		for (URLInventoryItemFragment* Fragment : DynamicFragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
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

void URLInventoryItemInstance::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (IsValid(ItemDefinition))
	{
		ItemDefinition->GetOwnedGameplayTags(TagContainer);
	}
}
