// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "RLInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FRLInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTage = FGameplayTag();
};


/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FRLInputAction> AbilityInputActions;


public:
	/// 返回一个有相同Tag的InputAction
	/// @param Tag 
	/// @param bLogNotFound 
	/// @return 
	const UInputAction* FindAbilityInputAction(const FGameplayTag Tag,bool bLogNotFound = false) const;
	
	
};
