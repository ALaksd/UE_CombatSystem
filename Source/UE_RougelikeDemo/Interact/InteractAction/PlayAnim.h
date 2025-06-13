// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractActionBase.h"
#include "PlayAnim.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UPlayAnim : public UInteractActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<USkeletalMeshComponent*, UAnimSequence*> SkeletalToAnimMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<USkeletalMeshComponent*, UAnimMontage*> SkeletalToMontageMap;

	virtual void Execute(AActor* Interactor, AActor* InteractedTarget) override;
};
