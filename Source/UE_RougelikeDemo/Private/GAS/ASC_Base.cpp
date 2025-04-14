// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ASC_Base.h"

void UASC_Base::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UASC_Base::EffectApplied);
}

void UASC_Base::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (auto AbilityClass : StartupAbilities)
	{
		const FGameplayAbilitySpec& AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		GiveAbility(AbilitySpec);
		
	}
}

void UASC_Base::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GamePlayEffectSpec,
	FActiveGameplayEffectHandle EffectHandle)
{
	
}
