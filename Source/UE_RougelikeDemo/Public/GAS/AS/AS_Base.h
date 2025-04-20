// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AS_Base.generated.h"


//定义自动Getter，Setter方法
//attribute_accessors
#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UAS_Base : public UAttributeSet
{
	GENERATED_BODY()
	

public:
	UAS_Base();


	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health,Category = "Attribute")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAS_Base, Health);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxHealth,Category = "Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAS_Base, MaxHealth);


	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth) const; 
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;


protected:
	//属性将修改前调用
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	

	//网络更新的，此项目不管
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps)const override;
	//属性修改前调用
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)override;
	//属性修改后调用
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)override;


	
};
