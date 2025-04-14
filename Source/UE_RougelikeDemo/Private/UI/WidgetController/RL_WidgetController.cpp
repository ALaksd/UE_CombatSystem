// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/RL_WidgetController.h"

void URL_WidgetController::SetWidgetControllerParams(FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void URL_WidgetController::BroadcastInitialValue()
{
}

void URL_WidgetController::BindCallbacksToDependencies()
{
}
