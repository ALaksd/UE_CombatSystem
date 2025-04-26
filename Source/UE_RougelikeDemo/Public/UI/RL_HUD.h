// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/RL_OverlayWidgetController.h"
#include "UI/WidgetController/RL_AttributeWidgetController.h"
#include "UI/WidgetController/RL_InventoryWidgetController.h"
#include "UI/WidgetController/RL_EquipWidgetController.h"
#include "RL_HUD.generated.h"

class URL_UserWidget;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API ARL_HUD : public AHUD
{
	GENERATED_BODY()

public:
	//返回控件控制器
	URL_OverlayWidgetController* GetOverlayWidgetController(FWidgetControllerParams& WCParams);

	URL_AttributeWidgetController* GetAttributeWidgetController(FWidgetControllerParams& WCParams);

	URL_InventoryWidgetController* GetInventoryWidgetController(FWidgetControllerParams& WCParams);

	URL_EquipWidgetController* GetEquipWidgetController(FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:
	//控件部件
	UPROPERTY()
	TObjectPtr<URL_UserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URL_UserWidget> OverlayWidgetClass;

	//控制器
	UPROPERTY()
	TObjectPtr<URL_OverlayWidgetController> OverlayController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URL_OverlayWidgetController> OverlayControllerClass;

	//控制器
	UPROPERTY()
	TObjectPtr<URL_AttributeWidgetController> AttributeController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URL_AttributeWidgetController> AttributeControllerClass;

	//控制器
	UPROPERTY()
	TObjectPtr<URL_InventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URL_InventoryWidgetController> InventoryWidgetControllerClass;

	//控制器
	UPROPERTY()
	TObjectPtr<URL_EquipWidgetController> EquipWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URL_EquipWidgetController> EquipWidgetControllerClass;
};
