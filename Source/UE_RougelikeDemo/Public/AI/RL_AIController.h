// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "RL_AIController.generated.h"

class UBehaviorTreeComponent;
class UAIPerceptionComponent;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API ARL_AIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ARL_AIController();
protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	/** AIPerception */
	// AI 感知组件
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;


	// 感知配置 - 视觉
	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	// 感知配置 - 听觉
	UPROPERTY()
	UAISenseConfig_Hearing* HearingConfig;

	// 感知更新回调
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	// 视觉感知配置
	UPROPERTY(EditAnywhere,Category = "AI|Perception|Sight")
	float SightRadius = 2000.f;

	UPROPERTY(EditAnywhere,Category = "AI|Perception|Sight")
	float LoseSightRadius = 3000.f;

	UPROPERTY(EditAnywhere,Category = "AI|Perception|Sight")
	float SightAngle = 60.f;

	// 听觉感知配置
	UPROPERTY(EditAnywhere, Category = "AI|Perception|Hearing")
	float HearingThreshold = 3000.f;
};
