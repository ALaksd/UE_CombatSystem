// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/RL_WidgetController.h"
#include "GAS/AS/AS_Player.h"
#include <Player/RL_PlayerState.h>

#include "GAS/ASC_Base.h"

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

void URL_WidgetController::UnbindAllDelegate()
{
}

UAS_Player* URL_WidgetController::GetPlayerAS() const
{
	UAS_Player* PlayerAttributeSet = CastChecked<UAS_Player>(AttributeSet);

	return PlayerAttributeSet;
}

inline ARL_PlayerState* URL_WidgetController::GetPlayerState() const
{
	ARL_PlayerState* RLPlayerState = CastChecked<ARL_PlayerState>(PlayerState);
	return RLPlayerState;
}

UASC_Base* URL_WidgetController::GetPlayerASC() const
{
	if (AbilitySystemComponent)
	{
		UASC_Base* ASC = Cast<UASC_Base>(AbilitySystemComponent);
		return ASC;
	}
	else
	{
		UASC_Base* ASC = Cast<UASC_Base>(GetPlayerState()->GetAbilitySystemComponent());
		return ASC;
	}
}
