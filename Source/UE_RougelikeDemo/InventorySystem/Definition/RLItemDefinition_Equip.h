// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemDefinition.h"
#include "RLItemDefinition_Equip.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLItemDefinition_Equip : public URLInventoryItemDefinition
{
	GENERATED_BODY()
	
public:
	URLItemDefinition_Equip(const FObjectInitializer& ObjectInitializer);
};
