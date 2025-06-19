// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effect/MMC_EnemyMaxResilience.h"

#include "GAS/AS/AS_Enemy.h"

UMMC_EnemyMaxResilience::UMMC_EnemyMaxResilience()
{
	//获取要用的属性
	MaxResilienceDef.AttributeToCapture = UAS_Enemy::GetResilienceAttribute();
	MaxResilienceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxResilienceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MaxResilienceDef);
}

float UMMC_EnemyMaxResilience::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 属性 = 基础属性 * (1 + 等级 * 成长率)
	// 获取敌人等级
	float Level = Spec.GetLevel();
	
	// 获取Tag容器
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	
	// 捕获属性
	float Resilience = 0.f;

	GetCapturedAttributeMagnitude(MaxResilienceDef, Spec, EvaluateParameters, Resilience);

	return Resilience*(1+Level*0.5);	
}
