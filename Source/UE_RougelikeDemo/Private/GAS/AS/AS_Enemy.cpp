// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Enemy.h"

void UAS_Enemy::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetResilienceAttribute())
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxResilience());
	
}
