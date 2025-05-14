// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_FindAttackPosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "AIController.h"
#include <GAS/RL_AbilitySystemLibrary.h>


EBTNodeResult::Type UBTSerivice_FindAttackPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;


	//设置位置
	FVector TargetLocation;
	FVector ControlledPawnLocation = ControlledPawn->GetActorLocation();
	FRotator TargetRotation;
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	float TargetDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TargetKey.SelectedKeyName);

	if (Target)
	{
		TargetLocation = Target->GetActorLocation();
		TargetRotation = (ControlledPawn->GetActorLocation() - Target->GetActorLocation()).Rotation();
	}


	FName TagName = OwnerComp.GetBlackboardComponent()->GetValueAsName(SelectedSkillKey.SelectedKeyName);
	if (TagName == "None")
		return EBTNodeResult::Failed;

	FEnemySkills SelectedSkill = URL_AbilitySystemLibrary::GetEnemyConfig(ControlledPawn)->FindSkillsByTag(FGameplayTag::RequestGameplayTag(TagName));

	//如果已经在攻击范围内，立即攻击
	if (TargetDistance >= SelectedSkill.SkillRangeMin && TargetDistance <= SelectedSkill.SkillRangeMax)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(
			AttackPositionKey.SelectedKeyName,
			ControlledPawnLocation);
		return EBTNodeResult::Succeeded;
	}

	else
	{
		// 生成基于目标位置的随机点
		FVector RandomTargetPos = GenerateSkillPositionAroundTarget(
			TargetLocation,
			SelectedSkill,
			TargetRotation
		);


		
		// 设置到黑板
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(
			AttackPositionKey.SelectedKeyName,
			RandomTargetPos
		);

	}

	return EBTNodeResult::Succeeded;

}

FVector UBTSerivice_FindAttackPosition::GenerateSkillPositionAroundTarget(const FVector& TargetLocation,const FEnemySkills& Skill,const FRotator& TargetRotation) const  // 新增参数：目标的旋转
{
	// 获取目标的正前方方向
	const FVector ForwardDir = TargetRotation.Vector().GetSafeNormal2D();

	// 在技能范围内随机距离
	const float RandomDistance = FMath::FRandRange(Skill.SkillRangeMin, Skill.SkillRangeMax);

	// 生成基础位置（玩家正前方）
	FVector FinalPos = TargetLocation + (ForwardDir * RandomDistance);

	// 保持与目标相同高度
	FinalPos.Z = TargetLocation.Z;

	if (bDrawDebug)
	{
		DrawDebugSphere(
			GetWorld(),
			FinalPos,
			50.0f,
			12,
			FColor::Green,
			false,
			2.0f
		);
	}

	return FinalPos;
}