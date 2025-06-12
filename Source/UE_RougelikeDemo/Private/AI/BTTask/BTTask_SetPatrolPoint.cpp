// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SetPatrolPoint.h"
#include "AIController.h"
#include "Components/SplineComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetPatrolPoint::UBTTask_SetPatrolPoint()
{
	bCreateNodeInstance = true;
	NodeName = "Set Patrol Point";
}

EBTNodeResult::Type UBTTask_SetPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	URL_EnemyMovementComponent* EnemyMove = OwnerPawn->FindComponentByClass<URL_EnemyMovementComponent>();

	if(!EnemyMove)
		return EBTNodeResult::Failed;
	
	FPatrolConfig PatrolConfig = EnemyMove->GetPatrolConfig();

	if (!PatrolConfig.bUseSplinePatrol)
		return EBTNodeResult::Failed;

	if (EnemyMove->GetPatrolPoints().Num() == 0)
		return EBTNodeResult::Succeeded;

	OwnerComp.GetBlackboardComponent()->SetValueAsVector("PatrolLocation", EnemyMove->GetPatrolPoints()[PatrolIndex]);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat("PatrolWaitTime", PatrolConfig.WaitTimeAtPoint);

	// 如果开启了巡逻路径循环
	if (PatrolConfig.bLoopPath)
	{
		// 判断巡逻点的索引方向
		if (PatrolIndex == EnemyMove->GetPatrolPoints().Num() - 1)
		{
			// 到达最后一个巡逻点时，反向巡逻
			PatrolIndexDirection = -1;
		}
		else if (PatrolIndex == 0)
		{
			// 到达第一个巡逻点时，恢复正向巡逻
			PatrolIndexDirection = 1;
		}

		// 根据巡逻方向更新 PatrolIndex
		PatrolIndex += PatrolIndexDirection;
	}
	else
	{
		// 如果没有开启循环路径，巡逻到最后一个点后结束任务
		if (PatrolIndex == EnemyMove->GetPatrolPoints().Num())
		{
			return EBTNodeResult::Succeeded;  // 任务成功完成，结束巡逻
		}

		// 否则，继续巡逻，正向增加 PatrolIndex
		PatrolIndex++;
	}

	return EBTNodeResult::Succeeded;
}
