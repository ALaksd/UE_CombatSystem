// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AS/AS_Base.h"
#include "AS_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UAS_Enemy : public UAS_Base
{
	GENERATED_BODY()

private:
	//韧性
	UPROPERTY(EditDefaultsOnly,Category = "Attribute")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAS_Enemy, Resilience);
	//韧性最大值
	UPROPERTY(EditDefaultsOnly,Category = "Attribute")
	FGameplayAttributeData MaxResilience;
	ATTRIBUTE_ACCESSORS(UAS_Enemy, MaxResilience);


private:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
};
