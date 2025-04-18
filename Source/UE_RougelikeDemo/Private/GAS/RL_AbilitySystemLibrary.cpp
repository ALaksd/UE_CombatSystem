// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RL_AbilitySystemLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "AbilitySystemComponent.h"
#include "Player/RL_PlayerState.h"
#include "UI/RL_HUD.h"
#include "UI/WidgetController/RL_AttributeWidgetController.h"
#include "UI/WidgetController/RL_OverlayWidgetController.h"

URL_OverlayWidgetController* URL_AbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARL_HUD* HUD = Cast<ARL_HUD>(PC->GetHUD()))
		{
			ARL_PlayerState* PS = PC->GetPlayerState<ARL_PlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			FWidgetControllerParams WidgetControllerParam(PC, PS, ASC, AS);
			return HUD->GetOverlayWidgetController(WidgetControllerParam);
		}
	}
	return nullptr;
}

URL_AttributeWidgetController* URL_AbilitySystemLibrary::GetAttributeMeauController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARL_HUD* HUD = Cast<ARL_HUD>(PC->GetHUD()))
		{
			ARL_PlayerState* PS = PC->GetPlayerState<ARL_PlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			FWidgetControllerParams WidgetControllerParam(PC, PS, ASC, AS);
			return HUD->GetAttributeWidgetController(WidgetControllerParam);
		}
	}
	return nullptr;
}