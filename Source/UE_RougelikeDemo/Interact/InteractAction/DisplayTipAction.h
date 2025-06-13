// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractActionBase.h"
#include "DisplayTipAction.generated.h"

class URLInventoryItemFragment;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UDisplayTipAction : public UInteractActionBase
{
	GENERATED_BODY()

public:

	virtual void Execute(AActor* Interactor, AActor* InteractedTarget) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> TipWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URLInventoryItemFragment> FragmentClass;
};
