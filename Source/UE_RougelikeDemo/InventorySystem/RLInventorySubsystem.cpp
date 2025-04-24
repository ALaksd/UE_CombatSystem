// Fill out your copyright notice in the Description page of Project Settings.


#include "RLInventorySubsystem.h"
#include "RLInventoryItemInstance.h"
#include "RLInventoryItemDefinition.h"

URLInventoryItemInstance* URLInventorySubsystem::GenerateItemInstance(URLInventoryItemDefinition* ItemDefinition)
{
	// 有效性验证
	if (!IsValid(ItemDefinition))
	{
		UE_LOG(LogTemp, Error, TEXT("GenerateItemInstance failed: Invalid ItemDefinition"));
		return nullptr;
	}

	// 创建实例
	URLInventoryItemInstance* NewInstance = NewObject<URLInventoryItemInstance>(this);
	NewInstance->SetItemDefinition(ItemDefinition);

	return NewInstance;
}