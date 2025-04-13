// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AS_Base.generated.h"


//增加各种Getter，Setter方法的宏
//attribute_accessors
#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(ProtertyName) \
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
	void OnRep_Health(const FGameplayAttributeData OldHealth); 
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps)const override;
	//属性修改前回调
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)override;
	//属性修改后回调
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)override;


};
