// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTService/BTService_PowerSkillDecision.h"
#include "BTS_PowerSkillDecision_Distance.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UBTS_PowerSkillDecision_Distance : public UBTService_PowerSkillDecision
{
	GENERATED_BODY()
	
public:
	UBTS_PowerSkillDecision_Distance();

protected:

	virtual bool CheckSkillCondition(const FEnemySkills& Skill, AAIController* AIController);
};
