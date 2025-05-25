// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_AttackDecision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include <Data/Enums.h>

UBTTask_AttackDecision::UBTTask_AttackDecision()
{
	NodeName = TEXT("AttackDecision");
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_AttackDecision::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;

	URL_EnemyMovementComponent* EnemyMove = ControlledPawn->FindComponentByClass<URL_EnemyMovementComponent>();
	if (!EnemyMove) return EBTNodeResult::Failed;

	const URL_EnemyConfig* EnemyConfig = EnemyMove->GetEnemyConfig();
	if (!EnemyConfig) return EBTNodeResult::Failed;

	// 获取当前与玩家的距离
	float TargetDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TargetDistanceKey.SelectedKeyName);
	const TArray<FEnemySkills>& WakingSkills = EnemyConfig->EnemySkills;

	TArray<FEnemySkills> ValidNormalSkills;

	// 分类收集可用技能（添加范围判断）
	for (const FEnemySkills& Skill : WakingSkills)
	{
		// 同时满足三个条件：
		// 1. 是普通技能
		// 2. 当前距离 >= 技能最小范围
		// 3. 当前距离 <= 技能最大范围
		if (!Skill.bIsPowerfulAttack &&
			TargetDistance >= Skill.SkillRangeMin &&
			TargetDistance <= Skill.SkillRangeMax)
		{
			ValidNormalSkills.Add(Skill);
		}
	}

	// 处理有效普通技能
	if (ValidNormalSkills.Num() > 0)
	{
		// 按优先级降序排序
		ValidNormalSkills.Sort([](const FEnemySkills& A, const FEnemySkills& B) {
			return A.PriorityLevel > B.PriorityLevel;
			});

		// 选择优先级最高的技能
		const FEnemySkills& SelectedSkill = ValidNormalSkills[0];
		OwnerComp.GetBlackboardComponent()->SetValueAsName(
			SelectedSkillKey.SelectedKeyName,
			SelectedSkill.AbilityTag.GetTagName());
	}
	else
	{
		// 没有有效技能时设置为None
		OwnerComp.GetBlackboardComponent()->SetValueAsName(
			SelectedSkillKey.SelectedKeyName,
			"None");
	}

	return EBTNodeResult::Succeeded;
}