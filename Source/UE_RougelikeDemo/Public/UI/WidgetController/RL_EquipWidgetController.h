// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/RL_WidgetController.h"
#include "RL_EquipWidgetController.generated.h"

class URLInventoryComponent_Equipment;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipItemSlotUpdate,URLInventoryItemInstance*, NewItem, URLInventoryItemInstance*, OldItem);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UE_ROUGELIKEDEMO_API URL_EquipWidgetController : public URL_WidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValue() override;

	virtual void BindCallbacksToDependencies() override;
	virtual void UnbindAllDelegate()override;

	UPROPERTY(BlueprintAssignable)
	FOnEquipItemSlotUpdate OnEquipItemSlotUpdate;

	UPROPERTY(BlueprintReadOnly)
	URLInventoryComponent_Equipment* EquipmentInventory;

	UFUNCTION()
	void HandleEquipUpdate(URLInventoryItemInstance* NewItem, URLInventoryItemInstance* OldItem);
};
