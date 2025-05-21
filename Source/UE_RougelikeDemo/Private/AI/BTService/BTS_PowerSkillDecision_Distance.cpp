// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTS_PowerSkillDecision_Distance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"


UBTS_PowerSkillDecision_Distance::UBTS_PowerSkillDecision_Distance()
{
	NodeName = "PowerSkillDecision_Distance";
}

bool UBTS_PowerSkillDecision_Distance::CheckSkillCondition(const FEnemySkills& Skill, AAIController* AIController)
{
	bool bPower = Skill.bIsPowerfulAttack;
	bool bNotCoolDown = !GetAbilitySystem(AIController->GetPawn())->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(FName(FString::Printf(TEXT("Cooldown.%s"), *Skill.AbilityTag.ToString())))
	);

	//获取距离
	float TargetDistance = AIController->GetBlackboardComponent()->GetValueAsFloat(TargetDistanceKey.SelectedKeyName);

	bool bInDistance = TargetDistance >= Skill.SkillRangeMin && TargetDistance <= Skill.SkillRangeMax;

	return bPower && bNotCoolDown && bInDistance;
}
