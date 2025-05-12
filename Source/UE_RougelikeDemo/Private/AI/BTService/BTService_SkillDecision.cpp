// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_SkillDecision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

UBTService_SkillDecision::UBTService_SkillDecision()
{
	
}

void UBTService_SkillDecision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	// 仅当无当前状态时选择新动作
	if (Blackboard->GetValueAsEnum(CurrentActionState.SelectedKeyName) == static_cast<uint8>(EEnemyActionState::None))
	{
		ChooseNewBaseAction(OwnerComp);
	}
}

// 简化后的基础行动选择
void UBTService_SkillDecision::ChooseNewBaseAction(UBehaviorTreeComponent& OwnerComp)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;

	// 获取 EnemyMovementComponent
	URL_EnemyMovementComponent* EnemyMove = ControlledPawn->FindComponentByClass<URL_EnemyMovementComponent>();
	if (!EnemyMove) return;

	// 获取配置的 EnemyDataAsset
	const URL_EnemyConfig* EnemyConfig = EnemyMove->GetEnemyConfig();

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	// 概率计算（示例值）
	const float Total = EnemyConfig->BaseActionWeights.EvadeChance + EnemyConfig->BaseActionWeights.AttackChance + EnemyConfig->BaseActionWeights.RollChance;
	const float RandomPick = FMath::FRand() * Total;

	// 设置黑板键值
	if (RandomPick <= EnemyConfig->BaseActionWeights.EvadeChance)
	{
		Blackboard->SetValueAsEnum(CurrentActionState.SelectedKeyName, static_cast<uint8>(EEnemyActionState::Evading));
	}
	else if (RandomPick <= (EnemyConfig->BaseActionWeights.EvadeChance + EnemyConfig->BaseActionWeights.AttackChance))
	{
		Blackboard->SetValueAsEnum(CurrentActionState.SelectedKeyName, static_cast<uint8>(EEnemyActionState::Attacking));
	}
	else
	{
		Blackboard->SetValueAsEnum(CurrentActionState.SelectedKeyName, static_cast<uint8>(EEnemyActionState::Rolling));
	}
}


