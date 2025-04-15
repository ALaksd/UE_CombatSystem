// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Player.h"

UAS_Player::UAS_Player()
{
	InitHealth(200.f);
	InitMaxHealth(200.f);
	InitEndurance(100.f);
	InitMaxEndurance(100.f);
	InitAttachResource(10.f);
	InitMaxAttachResource(10.f);
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
