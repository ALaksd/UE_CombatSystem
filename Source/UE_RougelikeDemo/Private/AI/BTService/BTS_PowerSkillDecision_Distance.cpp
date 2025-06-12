// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTS_PowerSkillDecision_Distance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include <GAS/RL_AbilitySystemLibrary.h>
#include "GAS/Abilities/GA_Base.h"
#include <AbilitySystemBlueprintLibrary.h>

UBTS_PowerSkillDecision_Distance::UBTS_PowerSkillDecision_Distance()
{
	NodeName = "PowerSkillDecision_Distance";
}

bool UBTS_PowerSkillDecision_Distance::CheckSkillCondition(const FEnemySkills& Skill, AAIController* AIController)
{
	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(AIController->GetPawn());

	bool bPower = Skill.bIsPowerfulAttack;
	bool bOwnerTagsMatch = Skill.RequiredOwnerTags.IsEmpty() || (OwnerASC && OwnerASC->HasAllMatchingGameplayTags(Skill.RequiredOwnerTags));
	bool bNotCoolDown = !GetAbilitySystem(AIController->GetPawn())->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(FName(FString::Printf(TEXT("Cooldown.%s"), *Skill.AbilityTag.ToString())))
	);
	// 获取技能的范围条件
	float TargetDistance = AIController->GetBlackboardComponent()->GetValueAsFloat(TargetDistanceKey.SelectedKeyName);
	bool bInDistance = TargetDistance >= Skill.SkillRangeMin && TargetDistance <= Skill.SkillRangeMax;


	// 返回所有判断条件
	return bPower && bNotCoolDown && bInDistance && bOwnerTagsMatch;
}

