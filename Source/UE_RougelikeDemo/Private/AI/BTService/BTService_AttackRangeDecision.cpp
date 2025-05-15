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

	// 获取目标对象
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor) return;

	// 获取当前使用的技能
	FGameplayTag CurrentSkillTag = FGameplayTag::RequestGameplayTag(
		Blackboard->GetValueAsName(SelectedSkillKey.SelectedKeyName)
	);

	// 如果未选择技能则使用默认
	if (!CurrentSkillTag.IsValid())
	{
		CurrentSkillTag = DefaultAttackTag;
		Blackboard->SetValueAsName(SelectedSkillKey.SelectedKeyName, DefaultAttackTag.GetTagName());
	}

	// 获取技能配置
	const FEnemySkills& SelectedSkill = URL_AbilitySystemLibrary::GetEnemyConfig(ControlledPawn)->FindSkillsByTag(CurrentSkillTag);

	// 计算实际距离
	const float CurrentDistance = FVector::Distance(
		ControlledPawn->GetActorLocation(),
		TargetActor->GetActorLocation()
	);

	// 判断是否在攻击范围
	const bool bCanAttack = (CurrentDistance >= SelectedSkill.SkillRangeMin) &&
		(CurrentDistance <= SelectedSkill.SkillRangeMax);

	// 设置黑板键
	Blackboard->SetValueAsBool(IsInAttackRangeKey.SelectedKeyName, bCanAttack);

	//if (bCanAttack)
	//{
	//	// 在范围内：设置攻击位置为当前位置
	//	Blackboard->SetValueAsVector(
	//		AttackPositionKey.SelectedKeyName,
	//		ControlledPawn->GetActorLocation()
	//	);
	//}
	//else
	//{
	//	// 不在范围：计算理想攻击位置
	//	const FRotator TargetRotation = (TargetActor->GetActorLocation() - ControlledPawn->GetActorLocation()).Rotation();
	//	const FVector IdealPosition = GenerateIdealAttackPosition(
	//		TargetActor->GetActorLocation(),
	//		SelectedSkill,
	//		TargetRotation
	//	);

	//	Blackboard->SetValueAsVector(
	//		AttackPositionKey.SelectedKeyName,
	//		IdealPosition
	//	);
	//}

	//// 调试绘制
	//if (bDrawDebug)
	//{
	//	DrawDebugSphere(
	//		GetWorld(),
	//		Blackboard->GetValueAsVector(AttackPositionKey.SelectedKeyName),
	//		50.0f,
	//		12,
	//		bCanAttack ? FColor::Green : FColor::Red,
	//		false,
	//		1.0f
	//	);
	//}
}

FVector UBTService_AttackRangeDecision::GenerateIdealAttackPosition(
	const FVector& TargetLocation,
	const FEnemySkills& SkillConfig,
	const FRotator& TargetRotation) const
{
	// 计算理想距离（取中间值增加成功率）
	const float IdealDistance = (SkillConfig.SkillRangeMin + SkillConfig.SkillRangeMax) * 0.5f;

	// 获取目标前方方向
	const FVector ForwardDir = TargetRotation.Vector().GetSafeNormal2D();

	// 生成基础位置
	FVector NewLocation = TargetLocation + (ForwardDir * IdealDistance);

	// 保持高度一致
	NewLocation.Z = TargetLocation.Z;

	return NewLocation;
}
