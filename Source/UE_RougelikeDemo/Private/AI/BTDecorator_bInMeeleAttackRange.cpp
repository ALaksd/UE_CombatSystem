// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_bInMeeleAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Component/RL_EnemyMovementComponent.h>
#include <GAS/RL_AbilitySystemLibrary.h>

UBTDecorator_bInMeeleAttackRange::UBTDecorator_bInMeeleAttackRange()
{
	NodeName = TEXT("Check Meele Distance");
	//NodeDescription = TEXT("判定攻击距离，与敌人配置表的近战攻击距离一致");
}

bool UBTDecorator_bInMeeleAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AActor* ControlledActor = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!ControlledActor) return false;

	const FEnemySkills& SelectedSkill = URL_AbilitySystemLibrary::GetEnemyConfig(ControlledActor)->FindSkillsByTag(FGameplayTag::RequestGameplayTag("Enemy.Ability.MeeleAttack"));

	float CurrentDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TargetDistanceKey.SelectedKeyName);

	const bool bCanAttack = CurrentDistance <= SelectedSkill.SkillRangeMax;

	return bCanAttack;
}
