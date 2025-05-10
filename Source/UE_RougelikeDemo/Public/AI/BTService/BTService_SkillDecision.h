// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "BTService_SkillDecision.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTService_SkillDecision : public UBTService_BlueprintBase
{
	GENERATED_BODY()

	// 缓存当前可用技能
	UPROPERTY()
	TArray<FEnemySkills> AvailableSkills;

	// 技能选择逻辑
	void EvaluateSkills(AAIController* AIController, APawn* ControlledPawn);
	bool CheckSkillCondition(const FEnemySkills& Skill, AAIController* AIController);
	FEnemySkills SelectSkillByProbability(const TArray<FEnemySkills>& ValidSkills);

	// GAS相关
	UAbilitySystemComponent* GetAbilitySystem(APawn* Pawn) const;

public:
	UBTService_SkillDecision();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelectedSkillKey;
};
