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
	UAS_Player();
	/*主要属性，实时*/
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	FGameplayAttributeData Life;  //生命，影响最大生命上限

	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	FGameplayAttributeData Vigor;  //活力，影响最大耐力上限

	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	FGameplayAttributeData Power;  //气势，影响最大气势上限
	
	/*次要属性，无限,受主要属性影响*/
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	FGameplayAttributeData MaxEndurance;
	ATTRIBUTE_ACCESSORS(UAS_Player, MaxEndurance); //最大耐力值

	//气势条最大值
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	FGameplayAttributeData MaxAttachResource;
	ATTRIBUTE_ACCESSORS(UAS_Player, MaxAttachResource);		//气势条最大值

	/*重要属性,无限,受次要属性影响*/
	//当前耐力值
	UPROPERTY(EditDefaultsOnly,Category="Attribute")
	FGameplayAttributeData Endurance;
	ATTRIBUTE_ACCESSORS(UAS_Player,Endurance);

	//气势条   你们谁有更好的想法就改个名字吧
	UPROPERTY(EditDefaultsOnly,Category="Attribute")
	FGameplayAttributeData AttachResource;
	ATTRIBUTE_ACCESSORS(UAS_Player,AttachResource);
private:
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
};
