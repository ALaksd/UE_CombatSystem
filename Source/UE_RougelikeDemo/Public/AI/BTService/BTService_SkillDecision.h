﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "BTService_SkillDecision.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTService_SkillDecision : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTService_SkillDecision();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CurrentActionState;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PositionType;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector bBroken;


	UPROPERTY(EditAnywhere)
	FGameplayTagContainer BrokenTags;
};
