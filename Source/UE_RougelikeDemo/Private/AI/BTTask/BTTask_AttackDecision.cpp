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

	// 获取 EnemyMovementComponent
	URL_EnemyMovementComponent* EnemyMove = ControlledPawn->FindComponentByClass<URL_EnemyMovementComponent>();
	if (!EnemyMove) return EBTNodeResult::Failed;

	// 获取配置的 EnemyDataAsset
	const URL_EnemyConfig* EnemyConfig = EnemyMove->GetEnemyConfig();
	if (!EnemyConfig) return EBTNodeResult::Failed;

	// 获取技能列表
	const TArray<FEnemySkills>& WakingSkills = EnemyConfig->EnemySkills;

	TArray<FEnemySkills> NormalSkills;


	// 分类收集可用技能
	for (const FEnemySkills& Skill : WakingSkills)
	{
		if (!Skill.bIsPowerfulAttack)
			NormalSkills.Add(Skill);
	}

	// 处理普通技能
	if (NormalSkills.Num() > 0)
	{
		NormalSkills.Sort([](const FEnemySkills& A, const FEnemySkills& B) {
			return A.PriorityLevel > B.PriorityLevel;
			});

		// 如果有可用技能，选择第一个（优先级最高的）
		const FEnemySkills& SelectedSkill = NormalSkills[0];
		FString String = *SelectedSkill.AbilityTag.ToString();
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, String);
		OwnerComp.GetBlackboardComponent()->SetValueAsName(SelectedSkillKey.SelectedKeyName, SelectedSkill.AbilityTag.GetTagName());
	}
	else
	{
		// 如果没有可用技能，设置为 None
		OwnerComp.GetBlackboardComponent()->SetValueAsName(SelectedSkillKey.SelectedKeyName, "None");
	}

	return EBTNodeResult::Succeeded;
}