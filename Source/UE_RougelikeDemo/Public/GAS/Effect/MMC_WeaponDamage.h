// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_WeaponDamage.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UMMC_WeaponDamage : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_WeaponDamage();

	//最终伤害=武器伤害*技能倍率+buff增幅
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	//武器伤害
	FGameplayEffectAttributeCaptureDefinition WeaponDamageDef;
	//技能倍率
	FGameplayEffectAttributeCaptureDefinition SkillMultiplierDef;
	//buff增幅
	FGameplayEffectAttributeCaptureDefinition BuffDamageDef;
	
};
