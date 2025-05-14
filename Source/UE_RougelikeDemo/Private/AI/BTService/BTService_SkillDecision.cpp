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

	// 获取与目标距离
	const float DistanceToPlayer = Blackboard->GetValueAsFloat(TargetDistance.SelectedKeyName);

	// 根据距离动态调整可用动作
	float ActualEvadeChance = EnemyConfig->BaseActionWeights.EvadeChance;
	float ActualAttackChance = EnemyConfig->BaseActionWeights.AttackChance;
	float ActualRollChance = EnemyConfig->BaseActionWeights.RollChance;

	// 如果距离超过允许值，移除翻滚概率
	if (DistanceToPlayer > RollMaxDistance)
	{
		ActualRollChance = 0.0f;
	}

	// 计算实际总权重
	const float Total = ActualEvadeChance + ActualAttackChance + ActualRollChance;
	if (Total <= 0.0f) return;

	// 生成随机选择
	const float RandomPick = FMath::FRand() * Total;

	// 决策逻辑
	EEnemyActionState SelectedState = EEnemyActionState::None;

	if (RandomPick <= ActualEvadeChance)
	{
		SelectedState = EEnemyActionState::Evading;
	}
	else if (RandomPick <= (ActualEvadeChance + ActualAttackChance))
	{
		SelectedState = EEnemyActionState::Attacking;
	}
	else
	{
		// 仅当距离有效时允许进入翻滚
		SelectedState = (DistanceToPlayer <= RollMaxDistance) ?
			EEnemyActionState::Rolling :
			EEnemyActionState::Evading; // 保底逻辑
	}

	Blackboard->SetValueAsEnum(CurrentActionState.SelectedKeyName, static_cast<uint8>(SelectedState));
}


