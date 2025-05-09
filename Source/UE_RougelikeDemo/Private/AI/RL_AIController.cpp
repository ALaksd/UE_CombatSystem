// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RL_AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AIPerceptionSystem.h"
#include <Component/RL_EnemyMovementComponent.h>


ARL_AIController::ARL_AIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);

	// 创建感知组件
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SetPerceptionComponent(*AIPerceptionComponent);

	// 设置视觉感知配置
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = SightAngle;
	SightConfig->SetMaxAge(5.f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// 设置听觉感知配置
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = HearingThreshold;
	HearingConfig->SetMaxAge(5.f);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	// 绑定感知配置
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->ConfigureSense(*HearingConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// 感知回调
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ARL_AIController::OnPerceptionUpdated);
}


void ARL_AIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{

	if (!GetBlackboardComponent()) return;

	// 成功感知（视觉或听觉）
	if (Stimulus.WasSuccessfullySensed() && Actor->ActorHasTag("Player"))
	{
		GetBlackboardComponent()->SetValueAsBool(FName("bFindTarget"), true);
	}
	//else
	//{
	//		// 丢失感知（例如离开视野）
	//	UObject* CurrentTarget = GetBlackboardComponent()->GetValueAsObject(FName("TargetToFollow"));
	//	if (CurrentTarget == Actor)
	//	{
	//		GetBlackboardComponent()->SetValueAsBool(FName("bFindTarget"), false);
	//	}
	//}
}
