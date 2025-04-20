// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "RL_AttributeData.generated.h"


USTRUCT(BlueprintType)
struct FRLAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAttributeValue = 0;

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_AttributeData : public UDataAsset
{
	GENERATED_BODY()

public:
	FRLAttributeInfo FindAttributeInfoFromTag(const FGameplayTag& AttributeTag) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRLAttributeInfo> AttributeInformation;
};
