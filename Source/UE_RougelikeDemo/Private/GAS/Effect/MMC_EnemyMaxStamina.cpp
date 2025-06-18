// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effect/MMC_EnemyMaxStamina.h"

#include "GAS/AS/AS_Enemy.h"

UMMC_EnemyMaxStamina::UMMC_EnemyMaxStamina()
{
	//获取要用的属性
	MaxStaminaDef.AttributeToCapture = UAS_Enemy::GetStaminaAttribute();
	MaxStaminaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	MaxStaminaDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(MaxStaminaDef);
}

float UMMC_EnemyMaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 属性 = 基础属性 * (1 + 等级 * 成长率)
	// 获取敌人等级
	float Level = Spec.GetLevel();
	
	// 获取Tag容器
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	
	// 捕获属性
	float Stamina = 0.f;

  	GetCapturedAttributeMagnitude(MaxStaminaDef, Spec, EvaluateParameters, Stamina);

	return Stamina*(1+Level*0.5);	
}