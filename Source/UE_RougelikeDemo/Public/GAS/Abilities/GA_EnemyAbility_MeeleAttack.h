// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_Base.h"
#include <Data/RL_EnemyConfig.h>
#include "GA_EnemyAbility_MeeleAttack.generated.h"


class UGameplayEffect;
/**
 * 敌人能力基础
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGA_EnemyAbility_MeeleAttack : public UGA_Base
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
protected:
	UAnimMontage* SelectRandomAnimation(const FEnemySkills& Skills);

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

private:
	FTimerHandle FacingUpdateTimerHandle;

};
