// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemFragment.h"
#include "RLItemFragment_UI.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLItemFragment_UI : public URLInventoryItemFragment
{
	GENERATED_BODY()
	
public:

	//仓库显示UI
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UTexture2D> InventoryIcon;

	//装备界面显示UI
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UTexture2D> EquipIcon;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	FName Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	FString Description;
};
