// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindingNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include <Interface/RL_CombatInterface.h>

void UBTService_FindingNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 获取关键对象
	APawn* OwningPawn = AIOwner->GetPawn();
	AAIController* AIController = Cast<AAIController>(AIOwner);
	if (!IsValid(OwningPawn) || !IsValid(AIController)) return;

	// 确定目标标签
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	// 寻找最近的有效目标
	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	for (AActor* Actor : ActorsWithTag)
	{
		if (Actor->Implements<URL_CombatInterface>()) // 假设死亡目标有"Dead"标签
		{
			if (IRL_CombatInterface::Execute_isDead(Actor))
			{
				continue; // 跳过无效或死亡目标
			}
			
		}

		const float Distance = OwningPawn->GetDistanceTo(Actor);
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}

	// 距离限制检测
	bool bIsTargetValid = false;
	if (ClosestActor && ClosestDistance <= MaxTrackingDistance)
	{
		// 设置焦点到目标
		AIController->SetFocus(ClosestActor);
		bIsTargetValid = true;
	}
	else
	{
		// 超出距离或目标无效时清除焦点
		AIController->ClearFocus(EAIFocusPriority::Gameplay);
		ClosestActor = nullptr;
		ClosestDistance = -1.0f; // 用负值表示无效距离
	}

	// 更新黑板键值
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
	UBTFunctionLibrary::SetBlackboardValueAsBool(this, IsTargetValidSelector, bIsTargetValid);
}
