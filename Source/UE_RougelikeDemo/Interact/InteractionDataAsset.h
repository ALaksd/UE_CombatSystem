// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InteractionDataAsset.generated.h"

class UInteractActionBase;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UInteractionDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/** 当前交互对象的提示文本 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	FText InteractPrompt;

	/** 可配置的行为列表，按顺序执行 */
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Interact")
	TArray<UInteractActionBase*> InteractActions;
};
