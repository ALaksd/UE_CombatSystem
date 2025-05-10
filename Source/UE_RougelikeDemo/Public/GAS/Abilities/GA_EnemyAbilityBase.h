// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_Base.h"
#include "GA_EnemyAbilityBase.generated.h"

class UGameplayEffect;
/**
 * 敌人能力基础
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGA_EnemyAbilityBase : public UGA_Base
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	FORCEINLINE void SetAbilityMontage(UAnimMontage* InAbilityMontage) { AbilityMontage = InAbilityMontage; }
protected:
	UFUNCTION()
	void OnMontageCompleted();
	void UpdateFacingDirection();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Montage")
	TObjectPtr<UAnimMontage> AbilityMontage;

private:
	FTimerHandle FacingUpdateTimerHandle;
};
