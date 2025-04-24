// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_RougelikeDemo/InventorySystem/Fragments/RLInventoryFragment_Equipment.h"
#include "GAS/Abilities/GA_Base.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemInstance.h"

bool URLInventoryFragment_Equipment::GiveToAbilitySystem(UAbilitySystemComponent* ASC, FRLAbilitySet_GrantHandles* OutHandle, URLInventoryItemInstance* SoruceObject) const
{
	if (ASC == nullptr) return false;

	if (OutHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Out Grant Handle is not Vaild"));
		return false;
	}

	//添加能力
	for (const auto& AbilityToGrant : EquipmentAbilitySet.GrantAbilities)
	{
		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities on equipment fragment[%s] is not vaild"),*GetNameSafe(this));
			continue;
		}

		UGA_Base * AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGA_Base>();
		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SoruceObject;

		FGameplayAbilitySpecHandle AbilitySpecHandle;
		
		//这里暂时认为能力都是主动的
		AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

		OutHandle->AddAbilitySpecHandle(AbilitySpecHandle);
	}
	
	//添加GE
	for (const auto& EffectToGrant : EquipmentAbilitySet.GrantEffects)
	{
		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayEffect on equipment fragment[%s] is not vaild"), *GetNameSafe(this));
			continue;
		}

		
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpec = ASC->MakeOutgoingSpec(EffectToGrant.GameplayEffect, EffectToGrant.EffectLevel, EffectContextHandle);
		const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());

		OutHandle->AddGameplayEffectHandle(GameplayEffectHandle);
	}
	
	return true;
}
