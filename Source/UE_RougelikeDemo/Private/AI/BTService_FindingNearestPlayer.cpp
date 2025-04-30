// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindingNearestPlayer.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"

void UBTService_FindingNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();

	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	//寻找最近的目标
	float ClosesetDistance = TNumericLimits<float>::Max();
	AActor* ClosesetActor = nullptr;

	for (AActor* Actor : ActorsWithTag)
	{
		GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Orange, *Actor->GetName());

		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosesetDistance)
			{
				ClosesetDistance = Distance;
				ClosesetActor = Actor;
			}
		}
	}
	//设置黑板键
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelecter, ClosesetActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelecter, ClosesetDistance);
}
