// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_Base.h"
#include "GA_HitReact.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGA_HitReact : public UGA_Base
{
	GENERATED_BODY()
	

public:
	UGA_HitReact();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> HitReactEffectClass;

private:
	UPROPERTY()
	FActiveGameplayEffectHandle ActiveHitReact;

	UFUNCTION()
	void OnMontageCompleted();
};
