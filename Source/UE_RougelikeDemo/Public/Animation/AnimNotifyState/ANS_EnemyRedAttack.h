// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifyState/ANS_EnemyAttackDecision.h"
#include "ANS_EnemyRedAttack.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UANS_EnemyRedAttack : public UANS_EnemyAttackDecision
{
	GENERATED_BODY()
	
public:
	UANS_EnemyRedAttack();
};
