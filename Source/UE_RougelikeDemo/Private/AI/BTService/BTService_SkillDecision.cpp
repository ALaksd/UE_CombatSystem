// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_SkillDecision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

UBTService_SkillDecision::UBTService_SkillDecision()
{
	
}

void UBTService_SkillDecision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	UAbilitySystemComponent* ASC = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UAbilitySystemComponent>();

	//破防状态判断
	if (ASC->HasAnyMatchingGameplayTags(BrokenTags))
	{
		Blackboard->SetValueAsBool(bBroken.SelectedKeyName, true);
	}
	else
	{
		Blackboard->SetValueAsBool(bBroken.SelectedKeyName, false);
	}

	//玩家位置信息更新
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Target.SelectedKeyName));
	AActor* ControllerActor = OwnerComp.GetAIOwner()->GetPawn();

	if (Player && ControllerActor)
	{
		FVector ToPlayer = Player->GetActorLocation() - ControllerActor->GetActorLocation();
		ToPlayer.Normalize();
		float DotForward = FVector::DotProduct(ControllerActor->GetActorForwardVector(), ToPlayer);
		float DotRight = FVector::DotProduct(ControllerActor->GetActorRightVector(), ToPlayer);

		EPositionType CurrentPostionType;
		// 判断方位类型
		if (DotForward > 0.7f) CurrentPostionType = EPositionType::Front;
		else if (DotRight > 0.5f) CurrentPostionType = EPositionType::Right;
		else if (DotRight < -0.5f) CurrentPostionType = EPositionType::Left;
		else CurrentPostionType = EPositionType::Back;

		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(PositionType.SelectedKeyName, static_cast<uint8>(CurrentPostionType));
	}

}

