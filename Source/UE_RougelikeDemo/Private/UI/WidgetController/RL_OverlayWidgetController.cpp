// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/RL_OverlayWidgetController.h"
#include "GAS/AS/AS_Player.h"
#include <Player/RL_PlayerState.h>

void URL_OverlayWidgetController::BroadcastInitialValue()
{
	OnHealthChanged.Broadcast(GetPlayerAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetPlayerAS()->GetMaxHealth());
	OnEnduranceChanged.Broadcast(GetPlayerAS()->GetEndurance());
	OnMaxEnduranceChanged.Broadcast(GetPlayerAS()->GetMaxEndurance());
	OnAttachResourceChanged.Broadcast(GetPlayerAS()->GetAttachResource());
	OnMaxAttachResourceChanged.Broadcast(GetPlayerAS()->GetMaxAttachResource());
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
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetEnduranceAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnEnduranceChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetMaxEnduranceAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxEnduranceChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetAttachResourceAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnAttachResourceChanged.Broadcast(Data.NewValue);
			}
		);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetMaxAttachResourceAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxAttachResourceChanged.Broadcast(Data.NewValue);
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
}
