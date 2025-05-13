// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "BTTask_FindAttackPosition.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTSerivice_FindAttackPosition : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;
	FVector GenerateSkillPositionAroundTarget(const FVector& TargetLocation, const FEnemySkills& Skill, const FRotator& TargetRotation) const;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelectedSkillKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SkillDistanceKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	bool bDrawDebug;
};
