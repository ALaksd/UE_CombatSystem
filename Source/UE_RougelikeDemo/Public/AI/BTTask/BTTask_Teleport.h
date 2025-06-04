// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Teleport.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTTask_Teleport : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Teleport();
	
public:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector LocationToGo;
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void TeleportTo(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation);

private:
	FVector Location;
	
};
