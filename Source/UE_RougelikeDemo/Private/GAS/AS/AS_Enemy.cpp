// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Enemy.h"

#include "Character/Enemy_Base.h"

void UAS_Enemy::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	// if (Attribute == GetResilienceAttribute())
	// 	NewValue = FMath::Clamp(NewValue,0.f,GetMaxResilience());
	
}

void UAS_Enemy::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (!EnemyRef)
		EnemyRef = CastChecked<AEnemy_Base>(GetOwningActor());
	
	// 体力相
	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxStamina());
		if (EnemyRef)
			EnemyRef->StaminaReduceCallBack();
		if (NewValue == 0)
		{
			// 进入破防状态
			if (EnemyRef)
			{
				EnemyRef->GuardBroken();
			}
		}
	}
	
	// 韧性
	if (Attribute == GetResilienceAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxResilience());

		if (NewValue == 0)
		{
			// 进入蹒跚状态
			if (EnemyRef)
			{
				EnemyRef->Staggered();
			}
		}
	}

}

void UAS_Enemy::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
