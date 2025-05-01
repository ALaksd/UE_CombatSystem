// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RL_ItemInstanceHolder.generated.h"

class URLInventoryItemInstance;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URL_ItemInstanceHolder : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_ROUGELIKEDEMO_API IRL_ItemInstanceHolder
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	URLInventoryItemInstance* GetItemInstance() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void SetItemInstance(URLInventoryItemInstance* NewInstance);
};
