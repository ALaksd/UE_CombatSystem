// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RL_HUD.h"
#include "UI/Widget/RL_UserWidget.h"
#include "UI/WidgetController/RL_LanternFlameController.h"


URL_OverlayWidgetController* ARL_HUD::GetOverlayWidgetController(FWidgetControllerParams& WCParams)
{
	if (OverlayController == nullptr)
	{
		OverlayController = NewObject<URL_OverlayWidgetController>(this, OverlayControllerClass);
		OverlayController->SetWidgetControllerParams(WCParams);
		OverlayController->BindCallbacksToDependencies();
		return OverlayController;
	}
	return OverlayController;
}

URL_AttributeWidgetController* ARL_HUD::GetAttributeWidgetController(FWidgetControllerParams& WCParams)
{
	if (AttributeController == nullptr)
	{
		AttributeController = NewObject<URL_AttributeWidgetController>(this, AttributeControllerClass);
		AttributeController->SetWidgetControllerParams(WCParams);
		AttributeController->BindCallbacksToDependencies();

		return AttributeController;
	}
	return AttributeController;
}

URL_InventoryWidgetController* ARL_HUD::GetInventoryWidgetController(FWidgetControllerParams& WCParams)
{
	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<URL_InventoryWidgetController>(this, InventoryWidgetControllerClass);
		InventoryWidgetController->SetWidgetControllerParams(WCParams);
		InventoryWidgetController->BindCallbacksToDependencies();

		return InventoryWidgetController;
	}
	return InventoryWidgetController;
}

URL_EquipWidgetController* ARL_HUD::GetEquipWidgetController(FWidgetControllerParams& WCParams)
{
	if (EquipWidgetController == nullptr)
	{
		EquipWidgetController = NewObject<URL_EquipWidgetController>(this, EquipWidgetControllerClass);
		EquipWidgetController->SetWidgetControllerParams(WCParams);
		EquipWidgetController->BindCallbacksToDependencies();

		return EquipWidgetController;
	}
	return EquipWidgetController;
}

URL_LanternFlameController* ARL_HUD::GetLanternFlameWidgetController()
{
	if (LanternFlameWidgetController)
		return LanternFlameWidgetController;
	return nullptr;
}

URL_LanternFlameController* ARL_HUD::GetLanternFlameWidgetController(FWidgetControllerParams& WCParams)
{
	if (LanternFlameWidgetController == nullptr)
	{
		LanternFlameWidgetController = NewObject<URL_LanternFlameController>(this, LanternFlameWidgetControllerClass);
		LanternFlameWidgetController->SetWidgetControllerParams(WCParams);
		LanternFlameWidgetController->BindCallbacksToDependencies();

		return LanternFlameWidgetController;
	}
	return LanternFlameWidgetController;
}

void ARL_HUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class unintialized"));
	checkf(OverlayControllerClass, TEXT("Overlay Widgety Controller Class unintialized"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<URL_UserWidget>(Widget);

	FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	URL_OverlayWidgetController* OverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// 初始化一下存档点控制器
	GetLanternFlameWidgetController(WidgetControllerParams);
	//

	
	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidgetController->BroadcastInitialValue();

	Widget->AddToViewport();
}
