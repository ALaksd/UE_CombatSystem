// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "BTTask_FindAttackPosition.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTTask_FindAttackPosition : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FVector GenerateSkillPositionAroundTarget(const FVector& TargetLocation, const FEnemySkills& Skill, const FRotator& TargetRotation) const;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelectedSkillKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackPositionKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetDistanceKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	bool bDrawDebug;
};
