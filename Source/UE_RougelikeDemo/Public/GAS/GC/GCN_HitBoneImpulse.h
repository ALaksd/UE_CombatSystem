// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "GCN_HitBoneImpulse.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UGCN_HitBoneImpulse : public UGameplayCueNotify_Static
{
	GENERATED_BODY()
	
public:
	virtual void HandleGameplayCue(AActor* TargetActor, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters) override;
};
