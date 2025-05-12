// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTService/BTService_SkillDecision.h"
#include "UBTService_SkillDecision_Awake.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UUBTService_SkillDecision_Awake : public UBTService_SkillDecision
{
	GENERATED_BODY()
	
protected:
	//virtual bool CheckSkillCondition(const FEnemySkills& Skill, AAIController* AIController) override
	//{
	//	bool bBaseCondition = Super::CheckSkillCondition(Skill, AIController);
	//	return bBaseCondition && Skill.AllowedStates.HasTag(FGameplayTag::RequestGameplayTag("Sanity.State.Awake"));
	//}
};
