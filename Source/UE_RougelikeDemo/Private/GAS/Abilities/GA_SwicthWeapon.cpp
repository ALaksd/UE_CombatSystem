// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_SwicthWeapon.h"
#include <UE_RougelikeDemo/InventorySystem/InventoryComponent/RLInventoryComponent_Equipment.h>
#include <AbilitySystemBlueprintLibrary.h>

void UGA_SwicthWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	 //成功切换,如果玩家正在释放技能，则释放切手技
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorInfo->OwnerActor.Get()))
	{
		if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("Ability")))
		{
			Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		}
		else
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
		}
	}

}
