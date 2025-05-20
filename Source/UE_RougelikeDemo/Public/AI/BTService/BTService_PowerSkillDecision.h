// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "BTService_PowerSkillDecision.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTService_PowerSkillDecision : public UBTService
{
	GENERATED_BODY()
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;

	virtual bool CheckSkillCondition(const FEnemySkills& Skill, AAIController* AIController);
	UAbilitySystemComponent* GetAbilitySystem(APawn* Pawn) const;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelectedPowerSkillKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector CurrentActionState;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetDistanceKey;
};
