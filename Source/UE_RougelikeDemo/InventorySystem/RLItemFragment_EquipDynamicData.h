// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RLInventoryItemFragment.h"
#include "RLItemFragment_EquipDynamicData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponUpgrade, int32, NewLevel);
/**
 * 装备的动态数据，Owner为所有的ItemInstance，在创建时需要显示设置
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLItemFragment_EquipDynamicData : public URLInventoryItemFragment
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void AddLevel() const;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponUpgrade OnWeaponUpgrade;
	//暂时只有等级
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	mutable int32 CurrentLevel = 0;
};
