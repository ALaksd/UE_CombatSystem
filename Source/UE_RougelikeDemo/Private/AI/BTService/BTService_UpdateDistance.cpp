// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_UpdateDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTService_UpdateDistance::UBTService_UpdateDistance()
{
	NodeName = "Update Distance";
}

void UBTService_UpdateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetToFollow.SelectedKeyName));
	URL_EnemyMovementComponent* EnemyMove = OwnerPawn->FindComponentByClass<URL_EnemyMovementComponent>();

	if (EnemyMove && TargetActor)
	{
		const float Distance = FVector::Distance(OwnerPawn->GetActorLocation(),
			TargetActor->GetActorLocation());

		FConfrontationConfig ConfrontationCongig = EnemyMove->GetConfrontationConfig();

		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceToTargetSelector.SelectedKeyName, Distance);
	}


}
