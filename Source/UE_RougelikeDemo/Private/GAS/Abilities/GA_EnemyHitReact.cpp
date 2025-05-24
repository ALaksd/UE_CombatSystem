// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_EnemyHitReact.h"
#include "AbilitySystemComponent.h"

bool UGA_EnemyHitReact::CanbeHit()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	if (!ASC) return false;

	// 必须有以下任一状态
	FGameplayTagContainer AllowHitTags;
	AllowHitTags.AddTag(FGameplayTag::RequestGameplayTag("EnemyState.Staggered"));
	AllowHitTags.AddTag(FGameplayTag::RequestGameplayTag("EnemyState.GuardBroken"));

	// 不能有这个状态
	FGameplayTag ExecuteTag = FGameplayTag::RequestGameplayTag("EnemyState.Execute");

	const bool bHasAllowTag = ASC->HasAnyMatchingGameplayTags(AllowHitTags);
	const bool bIsExecuting = ASC->HasMatchingGameplayTag(ExecuteTag);

	return bHasAllowTag && !bIsExecuting;
}


