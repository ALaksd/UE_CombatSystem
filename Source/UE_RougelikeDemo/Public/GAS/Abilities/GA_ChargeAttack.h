// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_Base.h"
#include "GA_ChargeAttack.generated.h"

class UAbilityTask_WaitInputRelease;
class UAbilityTask_PlayMontageAndWait;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGA_ChargeAttack : public UGA_Base
{
	GENERATED_BODY()

public:
	UGA_ChargeAttack();

	// 动画
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ChargeReleaseMontage;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ChargeLoopMontage;

	// 当前蓄力值（会随着蓄力时间上升）
	float CurrentChargeTime = 0.f;

	float ScaledPower = 0.f;

	// 计时器
	FTimerHandle ChargeTimerHandle;

	// 最长蓄力时间（秒）
	UPROPERTY(EditDefaultsOnly)
	float MaxChargeTime = 3.f;

	// 基础攻击倍率
	UPROPERTY(EditDefaultsOnly)
	float BasePower = 0.f;

	// 最大倍率（蓄满）
	UPROPERTY(EditDefaultsOnly)
	float MaxPower = 2.f;

	UPROPERTY(EditDefaultsOnly)
	float PerTimeSanity = 2.f;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void TickCharging(); // 每帧蓄力增长

	UFUNCTION()
	void OnReleased(float TimeHeld);   // 玩家松手时调用

	void StopChargingAnimation();

	UFUNCTION()
	void OnMontageCompleted();

	bool bReleased;

	UPROPERTY()
	UAbilityTask_WaitInputRelease* InputReleaseTask;

	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* ChargeMontageTask;

	UPROPERTY()
	AActor* AvatarActor;


	UPROPERTY()
	UAbilitySystemComponent* WeaponASC;

	UPROPERTY()
	FActiveGameplayEffectHandle DamageActiveEffectHandle;
};
