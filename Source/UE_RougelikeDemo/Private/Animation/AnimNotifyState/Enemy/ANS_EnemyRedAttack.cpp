// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/Enemy/ANS_EnemyRedAttack.h"

#include "GameplayTagContainer.h"

UANS_EnemyRedAttack::UANS_EnemyRedAttack()
{
	DamageTypeTag = FGameplayTag::RequestGameplayTag("damage.Red");
}
