﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayEffect.h"
#include "RLInventoryItemDefinition.generated.h"

class URLInventoryItemFragment;

/**
 * 物品定义，因为物品的属性不会发生改变，所以基类为DataAsset。
 * 用GameplayTags去说明物品的类别和属性，比如武器带有Equipment和weapon标签。
 * 物品的属性就由Fragment数组去进行配置, 函数FindFragment可以根据输入的Fragment类找到对应的属性
 */

UCLASS(BlueprintType)
class UE_ROUGELIKEDEMO_API URLInventoryItemDefinition : public UPrimaryDataAsset,public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	URLInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const URLInventoryItemFragment* FindFragmentByClass(TSubclassOf<URLInventoryItemFragment> FragmentClass) const;

	//将自己的CombinedTags添加到输入参数TagContainer中
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	//用于安全地组合父蓝图和子蓝图中的标签的结构
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FInheritedTagContainer ItemTags;

	//用于存放不改变的Fragments
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "Inventory")
	TArray<TObjectPtr<URLInventoryItemFragment>> Fragments;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FText DisplayName;

	// 最大堆叠数量（默认1表示不可堆叠）
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxStack = 1;

	// 是否允许堆叠（根据标签或其他条件判断）
	bool IsStackable() const { return MaxStack > 1; }
};
