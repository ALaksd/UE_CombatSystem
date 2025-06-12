// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_DisableMove.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UANS_DisableMove : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> MoveIMC;

};
