// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_InputEnableAll.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UANS_InputEnableAll : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> DefaultIMC;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> DisableIMC;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> MoveIMC;
};
