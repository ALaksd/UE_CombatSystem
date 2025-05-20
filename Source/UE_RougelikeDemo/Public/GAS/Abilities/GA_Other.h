// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_Base.h"
#include "GA_Other.generated.h"

enum class E_SkillType : uint8;
/**
 *	通用技能GA
 *	播放动画然后监听动画通知
 *	根据通知返回的E_SkillType来应用相应的Effect
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGA_Other : public UGA_Base
{
	GENERATED_BODY()

public:
	//技能动画
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Datas")
	TObjectPtr<UAnimMontage> Montage;
	// 此技能要使用的Effct
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Datas")
	TSubclassOf<UGameplayEffect> GameEffect;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION()
	void OnMontageCompleted();

private:
	void InitAnimation();
	UFUNCTION()
	void OnReciveNotify(USkeletalMeshComponent* Mesh);
	
};
