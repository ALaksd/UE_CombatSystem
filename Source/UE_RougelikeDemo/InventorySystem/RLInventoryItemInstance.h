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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool GetbEquiped() { return bEquipped; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetbEquiped(bool bInEquipped) { bEquipped = bInEquipped; }

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


	//可动态修改的fragment,暂时没有用到
	UPROPERTY()
	TArray<URLInventoryItemFragment*> DynamicFragments;
};
