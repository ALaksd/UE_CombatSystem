// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ASC_Base.h"

#include "GAS/Abilities/GA_Base.h"

void UASC_Base::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UASC_Base::EffectApplied);
}

void UASC_Base::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (auto AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		if (const UGA_Base* Ability = Cast<UGA_Base>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(Ability->StartInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void UASC_Base::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return ;

	//检查是否有启动键与当前输入键相同的Ability，并且此能力现在没有启用则启动它
	for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (!AbilitySpec.IsActive())
			{
				//执行Ability
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UASC_Base::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return ;

	for(auto AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UASC_Base::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GamePlayEffectSpec,
                              FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer TagContainer;
	GamePlayEffectSpec.GetAllAssetTags(TagContainer);

	EffecctTags.Broadcast(TagContainer);
}
