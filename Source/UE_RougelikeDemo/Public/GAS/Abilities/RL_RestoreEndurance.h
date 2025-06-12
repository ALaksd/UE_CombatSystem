// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RL_RestoreEndurance.generated.h"

class UAS_Player;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_RestoreEndurance : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	/**
	 * 耐力恢复机制
	 */
	FTimerHandle DelayHandle;	//延迟计时器
	FTimerHandle TickHandle;	//Tick计时器

	UPROPERTY(EditDefaultsOnly)
	float RecoverDelay = 3.f; // N 秒延迟

	UPROPERTY(EditDefaultsOnly)
	float TickInterval = 1.f; //恢复间隔

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> RestoreEnduranceEffectClass; //恢复值，需要在GE_RestoreEndurance配置

	void OnEnduranceChanged(const FOnAttributeChangeData& Data);
	void StartRestore();
	void RestorerTick();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo,const FGameplayAbilityActivationInfo ActivationInfo,const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle,const FGameplayAbilityActorInfo* ActorInfo,const FGameplayAbilityActivationInfo ActivationInfo,bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	float LastEndurance;
	UAbilitySystemComponent* ASC;
	const UAS_Player* PlayerAS;
};
