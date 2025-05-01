// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutomationBlueprintFunctionLibrary.h"
#include "RL_AbilitySystemLibrary.generated.h"

class URL_OverlayWidgetController;
class URL_AttributeWidgetController;
class URL_InventoryWidgetController;
class URL_EquipWidgetController;

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_AbilitySystemLibrary : public UAutomationBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static URL_OverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_AttributeWidgetController* GetAttributeMeauController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_InventoryWidgetController* GetInventoryWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_EquipWidgetController* GetEquipWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_LanternFlameController* GetLanternFlameWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
};
