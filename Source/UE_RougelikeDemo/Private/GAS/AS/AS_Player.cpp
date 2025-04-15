// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Player.h"

UAS_Player::UAS_Player()
{
	
}

void UAS_Player::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	//限制属性不能超过最大值
	if (Attribute == GetEnduranceAttribute())
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxEndurance());
	else if (Attribute == GetAttachResourceAttribute())
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxAttachResource());

	
}
