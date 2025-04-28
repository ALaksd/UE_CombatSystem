// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MessagSystem/Message_Base.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryComponent.h"
#include "Message_GetItem.generated.h"

/**
 * 玩家获取物品的广播
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UMessage_GetItem : public UMessage_Base
{
	GENERATED_BODY()

public:
	UPROPERTY()
	URLInventoryItemInstance* ItemInstance;
	UPROPERTY()
	FRLInventoryItemSlotHandle SlotHandle;
};
