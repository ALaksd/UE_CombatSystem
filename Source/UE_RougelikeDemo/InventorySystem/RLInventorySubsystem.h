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


	//生成ItemIntance的物理表示，可用于在World生成可拾取物品，附加到角色身上等
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AItem_Pickup* SpawnItemActorFromInstance(URLInventoryItemInstance* ItemInstance, const FVector& Location);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AActor* SpawnItemActorFromDefinition(URLInventoryItemDefinition* ItemDefinition, const FVector& Location);
};
