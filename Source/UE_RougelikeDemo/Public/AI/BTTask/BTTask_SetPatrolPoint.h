// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTTask_SetPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
	//巡逻路径下标
	int32 PatrolIndex = 0;
	//巡逻方向
	int32 PatrolIndexDirection = 1;
public:
	UBTTask_SetPatrolPoint();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
