// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_FindAttackPosition.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "AIController.h"



void UBTSerivice_FindAttackPosition::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;

	// 获取 EnemyMovementComponent
	URL_EnemyMovementComponent* EnemyMove = ControlledPawn->FindComponentByClass<URL_EnemyMovementComponent>();
	if (!EnemyMove) return;

	//设置位置
	FVector TargetLocation;
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));

	if (Target)
		TargetLocation = Target->GetActorLocation();

	FName TagName = OwnerComp.GetBlackboardComponent()->GetValueAsName(SelectedSkillKey.SelectedKeyName);
	if (TagName == "None")
		return;

	FEnemySkills SelectedSkill = EnemyMove->GetEnemyConfig()->FindSkillsByTag(FGameplayTag::RequestGameplayTag(TagName));
	// 生成基于目标位置的随机点
	FVector RandomTargetPos = GenerateSkillPositionAroundTarget(
		TargetLocation,
		SelectedSkill,
		OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation() // 可选：传入自身位置用于方向计算
	);

	// 设置到黑板
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(
		SkillDistanceKey.SelectedKeyName,
		RandomTargetPos
	);
}

FVector UBTSerivice_FindAttackPosition::GenerateSkillPositionAroundTarget(const FVector& TargetLocation, const FEnemySkills& Skill, const FVector& SelfLocation) const
{

	//基于相对位置的扇形分布（更智能）
	const FVector ToSelfDir = (SelfLocation - TargetLocation).GetSafeNormal2D();
	const float AngleVariation = FMath::FRandRange(-60.f, 60.f); // 60度扇形
	const FVector RandomDir = ToSelfDir.RotateAngleAxis(AngleVariation, FVector::UpVector);
	const float RandomDistance = FMath::FRandRange(Skill.SkillRangeMin, Skill.SkillRangeMax);

    // 生成最终位置
    FVector FinalPos = TargetLocation + RandomDir * RandomDistance;

    // 高度调整（根据需求选择）
    FinalPos.Z = TargetLocation.Z; // 保持与目标相同高度

    return FinalPos;
}