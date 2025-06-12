// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_FireProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UAN_FireProjectile : public UAnimNotify
{
	GENERATED_BODY()

public:
	// 是否单发
	UPROPERTY(EditAnywhere, Category="Attribute")
	bool bIsOneFire;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
