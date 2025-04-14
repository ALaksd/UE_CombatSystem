// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/RL_WidgetController.h"
#include "RL_OverlayWidgetController.generated.h"

struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
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
	FOnHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attribute")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	//回调函数
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
};
