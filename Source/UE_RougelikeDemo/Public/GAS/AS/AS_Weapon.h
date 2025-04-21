// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AS_Weapon.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UAS_Weapon : public UAttributeSet
{
	GENERATED_BODY()

public:
	//武器伤害
	UPROPERTY(BlueprintReadOnly,Category = "Attribute")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAS_Weapon,Damage);

	//Buff加成
	UPROPERTY(BlueprintReadOnly,Category = "Attribute")
	FGameplayAttributeData SkillAmplification;
	ATTRIBUTE_ACCESSORS(UAS_Weapon,SkillAmplification);

	//招式倍率
	UPROPERTY(BlueprintReadOnly,Category = "Attribute")
	FGameplayAttributeData DamageMultiplier;
	ATTRIBUTE_ACCESSORS(UAS_Weapon,DamageMultiplier);


	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	
};
