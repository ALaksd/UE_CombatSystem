// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractActionBase.h"
#include "Interact_PlaySound.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UInteract_PlaySound : public UInteractActionBase
{
	GENERATED_BODY()

	virtual void Execute(AActor* Interactor, AActor* InteractedTarget) override;
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> Sound;
};
