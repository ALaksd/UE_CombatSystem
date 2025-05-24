// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_Base.h"
#include "AbilitySystemComponent.h"

void UGA_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.BounceBack"));
	ActorInfo->AbilitySystemComponent->SetTagMapCount(FGameplayTag::RequestGameplayTag("State.BounceBack"), 0);
}

void UGA_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
