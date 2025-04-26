// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RL_HUD.h"
#include "UI/Widget/RL_UserWidget.h"



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

void ARL_HUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class unintialized"));
	checkf(OverlayControllerClass, TEXT("Overlay Widgety Controller Class unintialized"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<URL_UserWidget>(Widget);

	FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	URL_OverlayWidgetController* OverlayWidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(OverlayWidgetController);
	OverlayWidgetController->BroadcastInitialValue();

	Widget->AddToViewport();
}
