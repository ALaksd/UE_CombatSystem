// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RL_AIController.generated.h"

class UBehaviorTreeComponent;
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
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
