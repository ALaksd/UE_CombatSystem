// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_SkillDecision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

UBTService_SkillDecision::UBTService_SkillDecision()
{
	NodeName = "Waking State AttackDecision";
}

void UBTService_SkillDecision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;

	// 获取 EnemyMovementComponent
	URL_EnemyMovementComponent* EnemyMove = ControlledPawn->FindComponentByClass<URL_EnemyMovementComponent>();
	if (!EnemyMove) return;

	// 获取配置的 EnemyDataAsset
	const URL_EnemyConfig* EnemyConfig = EnemyMove->GetEnemyConfige();
	if (!EnemyConfig) return;

	const TArray<FEnemySkills>& WakingSkills = EnemyConfig->WakingStateAttackSkills;

	AvailableSkills.Empty();

	for (const FEnemySkills& Skill : WakingSkills)
	{
		if (CheckSkillCondition(Skill, AIController))
		{
			AvailableSkills.Add(Skill);
		}
	}

	// 按优先级排序
	AvailableSkills.Sort([](const FEnemySkills& A, const FEnemySkills& B) {
		return A.PriorityLevel > B.PriorityLevel;
		});

	if (AvailableSkills.Num() > 0)
	{
		const FEnemySkills& SelectedSkill = SelectSkillByProbability(AvailableSkills);
		OwnerComp.GetBlackboardComponent()->SetValueAsName(SelectedSkillKey.SelectedKeyName, SelectedSkill.AbilityTag.GetTagName());
	}
}


void UBTService_SkillDecision::EvaluateSkills(AAIController* AIController, APawn* ControlledPawn)
{
	//可以添加评估逻辑
}

bool UBTService_SkillDecision::CheckSkillCondition(const FEnemySkills& Skill, AAIController* AIController)
{
	APawn* Pawn = AIController->GetPawn();
	UAbilitySystemComponent* ASC = GetAbilitySystem(Pawn);

	//// 检查标签条件
	//if (!ASC->MatchesGameplayTagQuery(Skill.ActivationQuery))
	//	return false;

	//可添加新的条件
	return true;
}

FEnemySkills UBTService_SkillDecision::SelectSkillByProbability(const TArray<FEnemySkills>& ValidSkills)
{
	// 分组按优先级
	TMap<int32, TArray<FEnemySkills>> PriorityGroups;
	for (const auto& Skill : ValidSkills)
	{
		PriorityGroups.FindOrAdd(Skill.PriorityLevel).Add(Skill);
	}

	// 选择最高优先级
	int32 HighestPriority = TNumericLimits<int32>::Min();
	for (const auto& Elem : PriorityGroups)
	{
		if (Elem.Key > HighestPriority)
		{
			HighestPriority = Elem.Key;
		}
	}

	const TArray<FEnemySkills>& TopGroup = PriorityGroups[HighestPriority];

	// 权重计算
	float TotalWeight = 0.0f;
	for (const auto& Skill : TopGroup)
	{
		TotalWeight += Skill.SelectionWeight;
	}

	float RandomPoint = FMath::FRandRange(0.0f, TotalWeight);
	for (const auto& Skill : TopGroup)
	{
		if (RandomPoint <= Skill.SelectionWeight)
		{
			return Skill;
		}
		RandomPoint -= Skill.SelectionWeight;
	}

	return TopGroup.Last(); // Fallback
}


UAbilitySystemComponent* UBTService_SkillDecision::GetAbilitySystem(APawn* Pawn) const
{
	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		return ASC;
	}
	return nullptr;
}

