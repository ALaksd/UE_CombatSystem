// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_SkillDecision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

UBTService_SkillDecision::UBTService_SkillDecision()
{
	NodeName = "SkillDecision";
}

void UBTService_SkillDecision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController->GetPawn();
	URL_EnemyMovementComponent* EnemyMove = ControlledPawn->FindComponentByClass<URL_EnemyMovementComponent>();

	if (EnemyMove)
	{
		UDataTable* SkillConfigTable = EnemyMove->GetSkillConfigTable();

		if (!SkillConfigTable || !ControlledPawn)
			return;

		// 每帧更新可用技能列表
		AvailableSkills.Empty();

		// 获取所有技能配置
		TArray<FSkillConfig*> AllSkills;
		SkillConfigTable->GetAllRows<FSkillConfig>("", AllSkills);

		// 筛选可用技能
		for (auto& SkillConfig : AllSkills)
		{
			if (CheckSkillCondition(*SkillConfig, AIController))
			{
				AvailableSkills.Add(*SkillConfig);
			}
		}

		// 按优先级排序
		AvailableSkills.Sort([](const FSkillConfig& A, const FSkillConfig& B) {
			return A.PriorityLevel > B.PriorityLevel;
			});

		// 优先执行最高优先级技能
		if (AvailableSkills.Num() > 0)
		{
			FSkillConfig SelectedSkill = SelectSkillByProbability(AvailableSkills);
			OwnerComp.GetBlackboardComponent()->SetValueAsName(SelectedSkillKey.SelectedKeyName,SelectedSkill.SkillTag.GetTagName()); 
		}
	}
}

void UBTService_SkillDecision::EvaluateSkills(AAIController* AIController, APawn* ControlledPawn)
{
	//可以添加评估逻辑
}

bool UBTService_SkillDecision::CheckSkillCondition(const FSkillConfig& Skill, AAIController* AIController)
{
	APawn* Pawn = AIController->GetPawn();
	UAbilitySystemComponent* ASC = GetAbilitySystem(Pawn);

	// 检查标签条件
	if (!ASC->MatchesGameplayTagQuery(Skill.ActivationQuery))
		return false;

	//可添加新的条件
	return true;
}

FSkillConfig UBTService_SkillDecision::SelectSkillByProbability(const TArray<FSkillConfig>& ValidSkills)
{
	// 按优先级分组
	TMap<int32, TArray<FSkillConfig>> PriorityGroups;
	for (const auto& Skill : ValidSkills)
	{
		PriorityGroups.FindOrAdd(Skill.PriorityLevel).Add(Skill);
	}

	// 从最高优先级组中选择
	int32 HighestPriority = TNumericLimits<int32>::Min();
	for (const auto& Elem : PriorityGroups)
	{
		if (Elem.Key > HighestPriority)
		{
			HighestPriority = Elem.Key;
		}
	}

	const TArray<FSkillConfig>& TopGroup = PriorityGroups[HighestPriority];

	// 计算总权重
	float TotalWeight = 0.0f;
	for (const auto& Skill : TopGroup)
	{
		TotalWeight += Skill.SelectionWeight;
	}

	// 概率选择
	float RandomPoint = FMath::FRandRange(0.0f, TotalWeight);
	for (const auto& Skill : TopGroup)
	{
		if (RandomPoint <= Skill.SelectionWeight)
		{
			return Skill;
		}
		RandomPoint -= Skill.SelectionWeight;
	}

	return TopGroup.Last();
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

