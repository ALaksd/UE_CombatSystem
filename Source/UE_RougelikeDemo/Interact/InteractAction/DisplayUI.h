// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/RL_UIManagerSubsystem.h"
#include "UE_RougelikeDemo/Interact/InteractActionBase.h"
#include "DisplayUI.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UDisplayUI : public UInteractActionBase
{
	GENERATED_BODY()

public:

	virtual void Execute(AActor* Interactor, AActor* InteractedTarget) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URL_UserWidget> InteractWidgetClass;
};
