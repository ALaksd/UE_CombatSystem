// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/AS/AS_Base.h"
#include "AS_Player.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UAS_Player : public UAS_Base
{
	GENERATED_BODY()

public:
	//当前耐力值
	UPROPERTY(EditDefaultsOnly,Category="Attribute")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UAS_Player,Endurance);
	//最大耐力值
	UPROPERTY(EditDefaultsOnly,Category="Attribute")
	FGameplayAttributeData MaxEndurance;
	ATTRIBUTE_ACCESSORS(UAS_Player,MaxEndurance);

	//气势条   你们谁有更好的想法就改个名字吧
	UPROPERTY(EditDefaultsOnly,Category="Attribute")
	FGameplayAttributeData AttachResource;
	ATTRIBUTE_ACCESSORS(UAS_Player,AttachResource);

	//气势条最大值
	UPROPERTY(EditDefaultsOnly,Category="Attribute")
	FGameplayAttributeData MaxAttachResource;
	ATTRIBUTE_ACCESSORS(UAS_Player,MaxAttachResource);

private:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
};
