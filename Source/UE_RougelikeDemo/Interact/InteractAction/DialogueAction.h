// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractActionBase.h"
#include "DialogueAction.generated.h"

class UDialogueUserWidget;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UDialogueAction : public UInteractActionBase
{
	GENERATED_BODY()

public:
	virtual void Execute(AActor* Interactor, AActor* InteractedTarget) override;

	//对话UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDialogueUserWidget> Widget;
};
