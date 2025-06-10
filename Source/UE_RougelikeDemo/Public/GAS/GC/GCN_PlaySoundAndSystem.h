// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GCN_PlaySoundAndSystem.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGCN_PlaySoundAndSystem : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	virtual void HandleGameplayCue(AActor* TargetActor, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USoundBase> Sound;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UNiagaraSystem> DefaultRotEffect;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UNiagaraSystem> AlignToNormalEffect;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> HitCameraShake;

	//设置时间膨胀，蓝图可选实现
	UFUNCTION(BlueprintImplementableEvent)
	void SetPauseEffect(AActor* TargetActor);

	//可以在蓝图新增任意在弹反期间做的逻辑
	UFUNCTION(BlueprintImplementableEvent)
	void ExecuteOtherst(AActor* Enemy,AActor* Character);

protected:

	//敌人的弹反阈值
	UPROPERTY(BlueprintReadWrite)
	float ParryThreshold;

	//敌人的当前攻击击退值
	UPROPERTY(BlueprintReadWrite)
	float CurrentKnockDistance;
};
