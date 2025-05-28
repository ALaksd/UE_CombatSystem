// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "ANS_InputDisableAll.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UANS_InputDisableAll : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, Category = "Input")
	FGameplayTagContainer TagsToBlock;
};
