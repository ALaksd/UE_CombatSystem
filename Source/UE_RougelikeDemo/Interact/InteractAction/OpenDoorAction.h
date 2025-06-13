// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UE_RougelikeDemo/Interact/InteractActionBase.h"
#include "OpenDoorAction.generated.h"

class URL_UserWidget;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UOpenDoorAction : public UInteractActionBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URL_UserWidget> InteractWidgetClass;
	
	UPROPERTY(EditDefaultsOnly)
	FVector TranslateOffset;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag KeyTag;
	
	virtual void Execute(AActor* Interactor, AActor* InteractedTarget) override;

	UPROPERTY(EditDefaultsOnly)
	bool IsPush;

	UPROPERTY(EditDefaultsOnly)
	bool IsKey;
};
