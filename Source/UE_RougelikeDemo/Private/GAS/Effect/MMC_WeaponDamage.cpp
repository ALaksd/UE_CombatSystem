// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Effect/MMC_WeaponDamage.h"

#include "GAS/AS/AS_Weapon.h"

UMMC_WeaponDamage::UMMC_WeaponDamage()
{
	//获取要用的属性
	WeaponDamageDef.AttributeToCapture = UAS_Weapon::GetDamageAttribute();
	WeaponDamageDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	WeaponDamageDef.bSnapshot = false;

	SkillMultiplierDef.AttributeToCapture = UAS_Weapon::GetDamageMultiplierAttribute();
	SkillMultiplierDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	SkillMultiplierDef.bSnapshot = false;

	BuffDamageDef.AttributeToCapture = UAS_Weapon::GetSkillAmplificationAttribute();
	BuffDamageDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BuffDamageDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(WeaponDamageDef);
	RelevantAttributesToCapture.Add(SkillMultiplierDef);
	RelevantAttributesToCapture.Add(BuffDamageDef);
}

float UMMC_WeaponDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 获取Tag容器
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	
	//  从武器捕获属性
	float WeaponDamage = 0.f, SkillMultiplier = 1.f, BuffDamage = 0.f;
    
	GetCapturedAttributeMagnitude(WeaponDamageDef, Spec, EvaluateParameters, WeaponDamage);
	GetCapturedAttributeMagnitude(SkillMultiplierDef, Spec, EvaluateParameters, SkillMultiplier);
	GetCapturedAttributeMagnitude(BuffDamageDef, Spec, EvaluateParameters, BuffDamage);

	// 获取武器等级
	//ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetASC->GetAvatarActor());
	//const int32 WeaponLevel = TargetCombatInterface ? TargetCombatInterface->GetWeaponLevel() : 1;

	//  计算最终伤害
	//const float BaseWeaponDamage = YourWeaponDamageCurve->GetFloatValue(WeaponLevel);
	return WeaponDamage * SkillMultiplier + BuffDamage;	
}

