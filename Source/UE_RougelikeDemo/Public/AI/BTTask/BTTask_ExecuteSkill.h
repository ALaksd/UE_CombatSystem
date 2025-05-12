// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecuteSkill.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTTask_ExecuteSkill : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ExecuteSkill();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SelectedSkillKey;

private:
	void OnAbilityEnded(UGameplayAbility* EndedAbility);

	UAbilitySystemComponent* GetAbilitySystem(APawn* Pawn) const;

	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;

	FDelegateHandle AbilityEndedDelegateHandle;
};
