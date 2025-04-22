// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RL_ItemDataAsset.generated.h"

UENUM(BlueprintType)
enum class EItemType :uint8
{
	Weapon,
	Equipment,
};
/**
 * 
 */
UCLASS(BlueprintType)
class UE_ROUGELIKEDEMO_API URL_ItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> SpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCanStack;
};
