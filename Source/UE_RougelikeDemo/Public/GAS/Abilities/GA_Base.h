// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Base.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGA_Base : public UGameplayAbility
{
	GENERATED_BODY()

public:
	//触发技能的输入Tag
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag StartInputTag;
	
public:
	//使用技能
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
};
