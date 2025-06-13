// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InteractActionBase.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class UE_ROUGELIKEDEMO_API UInteractActionBase : public UObject
{
	GENERATED_BODY()

public:
	/** 执行交互行为 */
	virtual void Execute(AActor* Interactor, AActor* InteractedTarget);
};
