// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Enemy.h"

void UAS_Enemy::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetResilienceAttribute())
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxResilience());
	
}

void UAS_Enemy::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	GEngine->AddOnScreenDebugMessage(-1,2,FColor::Blue,FString::Printf(TEXT("Health will decrease to %f"),NewValue));
}

void UAS_Enemy::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	
}
