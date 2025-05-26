// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GCN_MeeleAttack.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGCN_MeeleAttack : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	virtual void HandleGameplayCue(AActor* TargetActor, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> HitCameraShake;

	//设置卡肉效果，蓝图实现
	UFUNCTION(BlueprintImplementableEvent)
	void SetPauseEffect(AActor* Instigator);
};
