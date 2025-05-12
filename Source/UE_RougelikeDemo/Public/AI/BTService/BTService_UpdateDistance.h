// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "BTService_UpdateDistance.generated.h"

/**
 * 计算敌人与玩家的位置，更新敌人的位置信息
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTService_UpdateDistance : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateDistance();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FBlackboardKeySelector TargetToFollow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector DistanceToTargetSelector;
};
