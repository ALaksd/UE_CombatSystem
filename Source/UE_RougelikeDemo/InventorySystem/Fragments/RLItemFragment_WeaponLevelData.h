// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemFragment.h"
#include "RLItemFragment_WeaponLevelData.generated.h"


USTRUCT(BlueprintType)
struct FWeaponLevelData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NeededFragment;  //强化所需要的碎片

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayTag FragmentTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NeededCurrency;  //强化所需要的钱
};

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLItemFragment_WeaponLevelData : public URLInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	// 添加获取当前等级数据的函数
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon")
	FWeaponLevelData GetWeaponLevelData(int32 Level) const; // 改为返回结构体实例

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Level")
	TObjectPtr<UDataTable> WeaponLevelDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Level")
	int32 MaxLevel = 10;
};
