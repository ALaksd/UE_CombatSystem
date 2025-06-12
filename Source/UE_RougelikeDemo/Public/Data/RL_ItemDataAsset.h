// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "RL_ItemDataAsset.generated.h"


USTRUCT(BlueprintType, Blueprintable)
struct FItemDataInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class URL_Item> ItemClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class URL_ItemInfoWidget> ItemInfoWidget = nullptr;
};

USTRUCT(BlueprintType)
struct FItemDataInfoMappingTag
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag Tag = FGameplayTag();

	// FItemDataInfo Data Table
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> ItemDataInfo = nullptr;
};

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_ItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	const FItemDataInfo& FindItemDataInfoByTag(const FGameplayTag& Tag);
private:
	UPROPERTY(EditDefaultsOnly, Category = "Item Data")
	TArray<FItemDataInfoMappingTag> ItemDataAsset;

	template <typename T>
	const T* FindItemDataInfoRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template <typename T>
const T* URL_ItemDataAsset::FindItemDataInfoRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	if (!DataTable) { return nullptr; }
	const T* Row = DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	return Row;
}