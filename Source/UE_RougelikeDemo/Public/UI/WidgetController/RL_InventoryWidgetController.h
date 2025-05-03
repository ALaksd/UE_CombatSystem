// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/RL_WidgetController.h"
#include "RL_InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnInventoryItemSlotUpdate,
	URLInventoryComponent*, InventoryComponent,
	const FRLInventoryItemSlotHandle&, SlotHandle,
	URLInventoryItemInstance*, NewItem,
	URLInventoryItemInstance*, OldItem);
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UE_ROUGELIKEDEMO_API URL_InventoryWidgetController : public URL_WidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValue() override;

	virtual void BindCallbacksToDependencies() override;
	virtual void UnbindAllDelegate()override;

	UPROPERTY(BlueprintAssignable)
	FOnInventoryItemSlotUpdate OnInventoryItemSlotUpdate;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URLInventoryComponent> InventoryComponnet;

	UFUNCTION()
	void HandleItemSlotUpdate(URLInventoryComponent* InventoryComponent, const FRLInventoryItemSlotHandle& SlotHandle, URLInventoryItemInstance* NewItem, URLInventoryItemInstance* OldItem);

};
