// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindingNearestPlayer.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTService_FindingNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector IsTargetValidSelector;

	UPROPERTY(BlueprintReadOnly,EditAnywhere)
	float MaxTrackingDistance = 1500.f;
};
