// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_Base.h"
#include "GA_AttackBase.generated.h"

class UAS_Player;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGA_AttackBase : public UGA_Base
{
	GENERATED_BODY()
public:

	UGA_AttackBase();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly,Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	FGameplayTag TirggerTag;

	// 此次GA是否扣除理智值
	UPROPERTY(EditDefaultsOnly,Category = "Attack | Sanity")
	bool bIsReduceSanity = false;

	// 要扣除的值
	UPROPERTY(EditDefaultsOnly,Category = "Attack | Sanity")
	float ReduceValue;
	
	//霸体值,0为没有，100为轻霸体,200为强霸体
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float Dominance = 0.f;

	UFUNCTION()
	void OnMontageCompleted();

private:
	UPROPERTY()
	UAS_Player* AS;
};
