// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/RL_OverlayWidgetController.h"
#include "GAS/AS/AS_Player.h"

void URL_OverlayWidgetController::BroadcastInitialValue()
{
	const UAS_Player* PlayerAttributeSet = CastChecked<UAS_Player>(AttributeSet);

	OnHealthChanged.Broadcast(PlayerAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(PlayerAttributeSet->GetMaxHealth());
}

void URL_OverlayWidgetController::BindCallbacksToDependencies()
{
	const UAS_Player* PlayerAttributeSet = CastChecked<UAS_Player>(AttributeSet);

	//绑定GAS属性变化的委托
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetHealthAttribute()).AddUObject(this, &URL_OverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		PlayerAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &URL_OverlayWidgetController::MaxHealthChanged);
}

void URL_OverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void URL_OverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
