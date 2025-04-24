// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo\InventorySystem\RLInventoryItemFragment.h"
#include "RLInventoryFragment_Equipment.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLInventoryFragment_Equipment : public URLInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	virtual bool GiveToAbilitySystem(UAbilitySystemComponent* ASC, FRLAbilitySet_GrantHandles* OutHandle, URLInventoryItemInstance* SoruceObject) const;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category = "Inventory")
	FRL_ItemDefinition_AbilitySet EquipmentAbilitySet;
};
