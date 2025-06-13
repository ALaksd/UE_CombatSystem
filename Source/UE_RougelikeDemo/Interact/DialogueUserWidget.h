// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractActors/InteractableNPCActor.h"
#include "DialogueUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UDialogueUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	//对话NPC
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AInteractableNPCActor* NPC;

	UFUNCTION()
	void InitWidget(AInteractableNPCActor* InitNPC);
};
