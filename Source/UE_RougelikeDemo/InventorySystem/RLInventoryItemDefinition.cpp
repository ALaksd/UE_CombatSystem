// Fill out your copyright notice in the Description page of Project Settings.

#include "RLInventoryItemDefinition.h"
#include "RLInventoryItemFragment.h"

URLInventoryItemDefinition::URLInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
{
}

const URLInventoryItemFragment* URLInventoryItemDefinition::FindFragmentByClass(TSubclassOf<URLInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (URLInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}

void URLInventoryItemDefinition::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(ItemTags.CombinedTags);
}
