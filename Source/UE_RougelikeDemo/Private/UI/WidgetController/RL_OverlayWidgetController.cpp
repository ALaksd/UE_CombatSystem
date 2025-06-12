// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/RL_OverlayWidgetController.h"
#include "GAS/AS/AS_Player.h"
#include <Player/RL_PlayerState.h>

#include "Component/RL_MovementComponent.h"

void URL_OverlayWidgetController::BroadcastInitialValue()
{
	OnHealthChanged.Broadcast(GetPlayerAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetPlayerAS()->GetMaxHealth());
}

void URL_OverlayWidgetController::BindCallbacksToDependencies()
{
	const UAS_Player* PlayerAttributeSet = CastChecked<UAS_Player>(AttributeSet);

	//绑定GAS属性变化的委托
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	GetPlayerState()->OnSoulChanged.AddLambda(
		[this](int32 NewSoul)
		{
			OnSoulChanged.Broadcast(NewSoul);
		}
	);

	GetPlayerState()->OnLevelChanged.AddLambda(
		[this](int32 NewLevel)
		{
			OnLevelChanged.Broadcast(NewLevel);
		}
	);

	URL_MovementComponent* MovementComponent = PlayerController->GetPawn()->FindComponentByClass<URL_MovementComponent>();
	if (MovementComponent)
	{
		MovementComponent->OnSwitchItem.BindLambda([this](int32 Index)
		{
			OnSwitchItemSignature.Broadcast(Index);
		});
	}
}

void URL_OverlayWidgetController::UnbindAllDelegate()
{
	OnHealthChanged.Clear();
	OnMaxHealthChanged.Clear();
	OnLevelChanged.Clear();
	OnSoulChanged.Clear();
}
