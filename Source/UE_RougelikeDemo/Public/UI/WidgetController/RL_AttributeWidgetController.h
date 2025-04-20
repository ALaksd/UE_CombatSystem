// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/RL_WidgetController.h"
#include "Data/RL_AttributeData.h"
#include "RL_AttributeWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FRLAttributeInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoulChangedInAttribute, int32, NewSoul);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChangedInAttribute, int32, NewLevel);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UE_ROUGELIKEDEMO_API URL_AttributeWidgetController : public URL_WidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValue() override;

	virtual void BindCallbacksToDependencies() override;

	virtual void UnbindAllDelegate() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnSoulChangedInAttribute OnSoulChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnLevelChangedInAttribute OnLevelChanged;

	UPROPERTY(BlueprintReadWrite)
	bool CanUprade;

protected:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<URL_AttributeData> AttributeInfo;


	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
};
