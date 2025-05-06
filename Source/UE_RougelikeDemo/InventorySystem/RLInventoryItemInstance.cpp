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


void URLInventoryItemInstance::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (IsValid(ItemDefinition))
	{
		ItemDefinition->GetOwnedGameplayTags(TagContainer);
	}
}
