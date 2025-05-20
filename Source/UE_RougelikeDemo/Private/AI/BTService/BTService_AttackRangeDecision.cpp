// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_AttackRangeDecision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include <GAS/RL_AbilitySystemLibrary.h>

void UBTService_AttackRangeDecision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaTime);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = AIController ? AIController->GetPawn() : nullptr;

	if (!Blackboard || !ControlledPawn) return;

	float CurrentDistance = Blackboard->GetValueAsFloat(TargetDistanceKey.SelectedKeyName);

	FName SkillName = Blackboard->GetValueAsName(SelectedSkillKey.SelectedKeyName);
	if (SkillName != "None")
	{
		FGameplayTag CurrentSkillTag = FGameplayTag::RequestGameplayTag(SkillName);

		// 如果未选择技能则使用默认
		if (!CurrentSkillTag.IsValid())
		{
			CurrentSkillTag = DefaultAttackTag;
			Blackboard->SetValueAsName(SelectedSkillKey.SelectedKeyName, DefaultAttackTag.GetTagName());
		}

		// 获取技能配置
		const FEnemySkills& SelectedSkill = URL_AbilitySystemLibrary::GetEnemyConfig(ControlledPawn)->FindSkillsByTag(CurrentSkillTag);


		// 判断是否在攻击范围
		const bool bCanAttack = (CurrentDistance >= SelectedSkill.SkillRangeMin) &&
			(CurrentDistance <= SelectedSkill.SkillRangeMax);

		// 设置黑板键
		Blackboard->SetValueAsBool(IsInAttackRangeKey.SelectedKeyName, bCanAttack);

	}
	else
	{
		Blackboard->SetValueAsBool(IsInAttackRangeKey.SelectedKeyName, false);

	}
}
