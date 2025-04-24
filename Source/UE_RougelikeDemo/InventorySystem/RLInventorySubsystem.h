// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RLInventorySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	URLInventoryItemInstance* GenerateItemInstance(URLInventoryItemDefinition *ItemDefinition);
};
