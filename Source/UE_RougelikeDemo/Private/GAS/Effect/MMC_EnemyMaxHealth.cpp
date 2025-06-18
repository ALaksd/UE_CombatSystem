// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effect/MMC_EnemyMaxHealth.h"

#include "GAS/AS/AS_Enemy.h"

UMMC_EnemyMaxHealth::UMMC_EnemyMaxHealth()
{
	//获取要用的属性
	MaxHealthDef.AttributeToCapture = UAS_Enemy::GetHealthAttribute();
	MaxHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxHealthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MaxHealthDef);
}

float UMMC_EnemyMaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 属性 = 基础属性 * (1 + 等级 * 成长率)
	// 获取敌人等级
	float Level = Spec.GetLevel();
	
	// 获取Tag容器
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	
	// 捕获属性
	float Health = 0.f;

	GetCapturedAttributeMagnitude(MaxHealthDef, Spec, EvaluateParameters, Health);

	return Health*(1+Level*0.5);	
}
