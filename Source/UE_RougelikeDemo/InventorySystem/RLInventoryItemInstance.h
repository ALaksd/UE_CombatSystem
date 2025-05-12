// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagAssetInterface.h"
#include "RLInventoryItemInstance.generated.h"

class URLInventoryItemDefinition;
class URLInventoryItemFragment;

/**
 * 物品实例。实例中存着物品定义的引用，和动态的fragment，好让物品的一部分属性可以在实例中修改，比如耐久度等等。
 */
UCLASS(BlueprintType, Blueprintable)
class UE_ROUGELIKEDEMO_API URLInventoryItemInstance : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
	
public:
	/** 设置和获取Definition */
	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE URLInventoryItemDefinition* GetItemDefinition() const { return ItemDefinition; }
	FORCEINLINE void SetItemDefinition(URLInventoryItemDefinition* InDefinition) { ItemDefinition = InDefinition; }
	FORCEINLINE void AddDynamicFragments(URLInventoryItemFragment* NewFragments) { DynamicFragments.Add(NewFragments); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetbEquiped() { return bEquipped; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetbEquiped(bool bInEquipped) { bEquipped = bInEquipped; }

	/** 堆叠 */

	int32 GetCurrentStack() const { return CurrentStack; }
	// 最大堆叠数量（从定义中获取）
	int32 GetMaxStack() const;

	//增加堆叠数，返回不可堆叠剩下的数量
	int32 AddStack(int32 Quantity);

	void SetStack(int32 InitialQuantity = 1);

	/** 物品实例同样也需要GameplayTags去判断类别和属性 */
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	/** 根据fragment类获取对应的属性 */
	UFUNCTION(BlueprintCallable, BlueprintPure = "false", meta = (DeterminesOutputType = FragmentClass))
	const URLInventoryItemFragment* FindFragmentByClass(TSubclassOf<URLInventoryItemFragment> FragmentClass) const; 

	template<typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)(FindFragmentByClass(ResultClass::StaticClass()));
	}

protected:
	UPROPERTY(EditDefaultsOnly)
	URLInventoryItemDefinition* ItemDefinition;

	// 装备状态
	UPROPERTY()
	bool bEquipped = false;

	// 当前堆叠数量
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Inventory")
	int32 CurrentStack = 1;


	//可动态修改的fragments
	UPROPERTY()
	TArray<URLInventoryItemFragment*> DynamicFragments;
};
