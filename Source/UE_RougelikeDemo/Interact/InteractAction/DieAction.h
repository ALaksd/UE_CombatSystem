// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractActionBase.h"
#include "DieAction.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UDieAction : public UInteractActionBase
{
	GENERATED_BODY()

public:

	virtual void Execute(AActor* Interactor, AActor* InteractedTarget) override;
};
