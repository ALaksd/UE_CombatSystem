// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Enemy.h"

#include <string>

#include "Character/Enemy_Base.h"

void UAS_Enemy::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	// if (Attribute == GetResilienceAttribute())
	// 	NewValue = FMath::Clamp(NewValue,0.f,GetMaxResilience());
	
}

void UAS_Enemy::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//Super::PreAttributeChange(Attribute, NewValue);
	
	// 体力
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxStamina());
	}
	
	// 韧性
	if (Attribute == GetResilienceAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxResilience());
	}

}

void UAS_Enemy::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UAS_Enemy::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UAS_Enemy::HandleSpecialDamage(float& Damage)
{
	Super::HandleSpecialDamage(Damage);

	if (AEnemy_Base* Enemy = Cast<AEnemy_Base>(GetOwningActor()))
	{
		if (Enemy->bIsGuardBroken)
		{
			Damage = Damage * 1.2;
		}
		if (Enemy->GetbIsExectute())
		{
			Damage = Damage * 10;
		}
	}	
}
