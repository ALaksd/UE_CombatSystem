// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RL_Item.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
	FItemData() {}

	// 名字
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName = FName();

	// 图片
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> ItemIcon = nullptr;

	//最大堆叠数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 MaxStack = 20;

	// 当前数量
	UPROPERTY(BlueprintReadWrite)
	uint8 Amount = 0;

	// 描述
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description = FText();
};

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_Item : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Item Info")
	FORCEINLINE FItemData& GetItemData() { return ItemData; }

	UFUNCTION(BlueprintCallable, Category = "Item Info")
	const FItemData& UpdateAmount(const int32& Magnitude);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Item Info")
	FItemData ItemData;
};
