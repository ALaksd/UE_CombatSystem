// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_FindAttackPosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "AIController.h"
#include <GAS/RL_AbilitySystemLibrary.h>



void UBTSerivice_FindAttackPosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;


	//设置位置
	FVector TargetLocation;
	FRotator TargetRotation;
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));

	if (Target)
	{
		TargetLocation = Target->GetActorLocation();
		TargetRotation = Target->GetActorForwardVector().Rotation();
	}
		

	FName TagName = OwnerComp.GetBlackboardComponent()->GetValueAsName(SelectedSkillKey.SelectedKeyName);
	if (TagName == "None")
		return;

	FEnemySkills SelectedSkill = URL_AbilitySystemLibrary::GetEnemyConfig(ControlledPawn)->FindSkillsByTag(FGameplayTag::RequestGameplayTag(TagName));
	// 生成基于目标位置的随机点
	FVector RandomTargetPos = GenerateSkillPositionAroundTarget(
		TargetLocation,
		SelectedSkill,
		TargetRotation
	);



	// 设置到黑板
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(
		SkillDistanceKey.SelectedKeyName,
		RandomTargetPos
	);

	if (bDrawDebug)
	{
		DrawDebugSphere(
			GetWorld(),
			RandomTargetPos,
			50.0f,
			12,
			FColor::Green,
			false,
			2.0f
		);
	}
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

	return FinalPos;
}