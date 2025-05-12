// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AS/AS_Base.h"
#include "AS_Enemy.generated.h"

class AEnemy_Base;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UAS_Enemy : public UAS_Base
{
	GENERATED_BODY()

public:
	//体力
	UPROPERTY(EditDefaultsOnly,Category = "Attribute")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UAS_Enemy, Stamina);
	
private:
	//韧性
	UPROPERTY(EditDefaultsOnly,Category = "Attribute")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAS_Enemy, Resilience);
	//韧性最大值
	UPROPERTY(EditDefaultsOnly,Category = "Attribute")
	FGameplayAttributeData MaxResilience;
	ATTRIBUTE_ACCESSORS(UAS_Enemy, MaxResilience);

	
	//体力最大值
	UPROPERTY(EditDefaultsOnly,Category = "Attribute")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UAS_Enemy, MaxStamina);

	UPROPERTY()
	TObjectPtr<AEnemy_Base> EnemyRef;
	
private:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	//属性修改前调用
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)override;
	//属性修改后调用
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)override;
	
};
