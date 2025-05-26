// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemFragment.h"
#include "RLItemFragment_Attached.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLItemFragment_Attached : public URLInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	AActor* AttachToActor(AActor* Owner_T, URLInventoryItemInstance* ItemInstance) const;

	URLInventoryItemInstance* ArmorAttachToActor(AActor* Owner_T, URLInventoryItemInstance* ItemInstance) const;

	void RemoveArmor(AActor* Owner_T) const;

	UPROPERTY(EditDefaultsOnly,Category = "Item")
	TMap<FName, TSubclassOf<AActor>> AttachedMap;

};
