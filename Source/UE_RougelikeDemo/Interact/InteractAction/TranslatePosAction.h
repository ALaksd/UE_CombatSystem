// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractActionBase.h"
#include "TranslatePosAction.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UTranslatePosAction : public UInteractActionBase
{
	GENERATED_BODY()
	
public:

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TranslateTime;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector EndPos; 

	virtual void Execute(AActor* Interactor, AActor* InteractedTarget) override;
};
