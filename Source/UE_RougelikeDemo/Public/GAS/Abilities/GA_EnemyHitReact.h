// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/Abilities/GA_HitReact.h"
#include "GA_EnemyHitReact.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGA_EnemyHitReact : public UGA_HitReact
{
	GENERATED_BODY()
	
protected:
	virtual bool CanbeHit() override;
};
