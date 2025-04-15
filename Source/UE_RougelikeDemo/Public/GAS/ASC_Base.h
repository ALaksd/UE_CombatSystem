// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ASC_Base.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UASC_Base : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
protected:
	void EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GamePlayEffectSpec, FActiveGameplayEffectHandle EffectHandle);
	
};
