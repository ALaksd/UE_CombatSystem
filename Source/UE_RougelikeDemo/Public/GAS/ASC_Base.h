// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ASC_Base.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UASC_Base : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();
	void AddCharacterAbility(const TSubclassOf<UGameplayAbility> StartupAbilities);
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	//加属性
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	FEffectAssetTags EffecctTags;
		
	
protected:
	void EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GamePlayEffectSpec, FActiveGameplayEffectHandle EffectHandle);
	
};
