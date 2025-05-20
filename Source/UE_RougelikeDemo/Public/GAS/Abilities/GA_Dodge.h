// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_Base.h"
#include "GA_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGA_Dodge : public UGA_Base
{
	GENERATED_BODY()
	UGA_Dodge();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	UPROPERTY(EditDefaultsOnly,Category = "Defense")
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditDefaultsOnly,Category = "Defense")
	TObjectPtr<UAnimMontage> DodgeMontageF;

	UPROPERTY(EditDefaultsOnly,Category = "Defense")
	TObjectPtr<UAnimMontage> DodgeMontageB;

	UPROPERTY(EditDefaultsOnly,Category = "Defense")
	TObjectPtr<UAnimMontage> DodgeMontageL;

	UPROPERTY(EditDefaultsOnly,Category = "Defense")
	TObjectPtr<UAnimMontage> DodgeMontageR;

	UFUNCTION()
	void OnMontageCompleted();
};
