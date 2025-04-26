// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemFragment.h"
#include "RLItemFragment_Pickup.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLItemFragment_Pickup : public URLInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	// 物品生成出来所使用的Actor类
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Item")
	TSubclassOf<AActor> ItemActorClass;
};
