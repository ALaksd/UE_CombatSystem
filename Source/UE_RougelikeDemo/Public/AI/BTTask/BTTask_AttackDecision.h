		// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "BTTask_AttackDecision.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTTask_AttackDecision : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_AttackDecision();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelectedSkillKey;
};
