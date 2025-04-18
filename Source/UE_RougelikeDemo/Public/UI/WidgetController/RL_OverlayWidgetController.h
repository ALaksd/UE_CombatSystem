// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/RL_WidgetController.h"
#include "RL_OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoulChangedSignature, int32, NewSoul);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChangedSignature, int32, NewLevel);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UE_ROUGELIKEDEMO_API URL_OverlayWidgetController : public URL_WidgetController
{
	GENERATED_BODY()

public:
	//广播属性变化
	virtual void BroadcastInitialValue() override;

	//GAS回调函数(用于GAS通知属的变化委托的回调函数)
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSignature OnEnduranceChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSignature OnMaxEnduranceChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSignature OnAttachResourceChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnAttributeChangedSignature OnMaxAttachResourceChanged;

	UPROPERTY(BlueprintAssignable, Category = "LevelUpInfo")
	FOnSoulChangedSignature OnSoulChanged;

	UPROPERTY(BlueprintAssignable, Category = "LevelUpInfo")
	FOnLevelChangedSignature OnLevelChanged;
};
