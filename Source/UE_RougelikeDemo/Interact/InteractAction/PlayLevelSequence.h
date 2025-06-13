// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractActionBase.h"
#include "PlayLevelSequence.generated.h"

class ALevelSequenceActor;
class ULevelSequence;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UPlayLevelSequence : public UInteractActionBase
{
	GENERATED_BODY()

public:

	virtual void Execute(AActor* Interactor, AActor* InteractedTarget) override;

	UFUNCTION()
	void OnSequenceFinished();
	
	UPROPERTY(EditDefaultsOnly)
	ULevelSequence* SequenceToPlay;
	
private:
	
	UPROPERTY()
	ALevelSequenceActor* OutActor;
};
