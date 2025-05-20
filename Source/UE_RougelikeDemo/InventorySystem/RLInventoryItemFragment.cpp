// Fill out your copyright notice in the Description page of Project Settings.


#include "RLInventoryItemFragment.h"

void URLInventoryItemFragment::OnInstancedCreate(URLInventoryItemInstance* Instance) const
{
	Owner=Instance;
}

void FRLAbilitySet_GrantHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& AbilityHandle)
{
	if (AbilityHandle.IsValid())
	{
		AbilitySpecHandles.Add(AbilityHandle);
	}
}

void FRLAbilitySet_GrantHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& EffectHandle)
{
	if (EffectHandle.IsValid())
	{
		GameplayEffectHandles.Add(EffectHandle);
	}
}

void FRLAbilitySet_GrantHandles::TakeFromAbilitySystem(UASC_Base* ASC)
{
	if (ASC)
	{
		for (const auto& Handle : AbilitySpecHandles)
		{
			if (Handle.IsValid())
			{
				ASC->ClearAbility(Handle);
			}
		}

		for (const auto& Handle : GameplayEffectHandles)
		{
			if (Handle.IsValid())
			{
				ASC->RemoveActiveGameplayEffect(Handle);
			}
		}
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
}
