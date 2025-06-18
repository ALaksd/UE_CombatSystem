// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_EnemyMaxResilience.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UMMC_EnemyMaxResilience : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_EnemyMaxResilience();

	// 自定义计算
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	//韧性
	FGameplayEffectAttributeCaptureDefinition MaxResilienceDef;
	
};
