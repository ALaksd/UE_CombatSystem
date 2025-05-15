// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "BTService_AttackRangeDecision.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTService_AttackRangeDecision : public UBTService
{
	GENERATED_BODY()
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;

	FVector GenerateIdealAttackPosition(const FVector& TargetLocation, const FEnemySkills& SkillConfig, const FRotator& TargetRotation) const;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelectedSkillKey;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetDistanceKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category = "Attack")
	FBlackboardKeySelector IsInAttackRangeKey; // 新增键：是否在攻击范围

	UPROPERTY(EditAnywhere, Category = "Attack")
	FGameplayTag DefaultAttackTag; // 默认攻击技能标签

	UPROPERTY(EditAnywhere)
	bool bDrawDebug;
};
