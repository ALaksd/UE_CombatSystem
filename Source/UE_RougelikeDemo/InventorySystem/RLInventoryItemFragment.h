// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GAS/ASC_Base.h"
#include "RLInventoryItemFragment.generated.h"


class UGA_Base;
class UASC_Base;
/**
 * 技能结构体，暂时放在这里
 */
USTRUCT(BlueprintType)
struct UE_ROUGELIKEDEMO_API FRLAbilitySet_GameplayAbility
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	TSubclassOf<UGA_Base> Ability = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 AbilityLevel = 1;
};

USTRUCT(BlueprintType)
struct UE_ROUGELIKEDEMO_API FRLAbilitySet_GameplayEffect
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float EffectLevel = 1.0f;
};

/**
 * 用于管理和储存AbilitySet给予的Handles
 */
USTRUCT(BlueprintType)
struct UE_ROUGELIKEDEMO_API FRLAbilitySet_GrantHandles
{
	GENERATED_BODY()

public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& AbilityHandle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& EffectHandle);
	void TakeFromAbilitySystem(UASC_Base* ASC);
protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
};

/**
 * 技能集和属性集
 */

USTRUCT(BlueprintType)
struct UE_ROUGELIKEDEMO_API FRL_ItemDefinition_AbilitySet
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category = "Inventory")
	TArray<FRLAbilitySet_GameplayAbility> GrantAbilities;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	TArray<FRLAbilitySet_GameplayEffect> GrantEffects;
};
class URLInventoryItemInstance;
/**
 * 定义抽象的基类fragment来决定物品的属性，物品的属性由组件来决定
 * DefaultToInstanced:当该类的对象作为另一个对象的属性（UPROPERTY）时，引擎会自动创建该类的实例防止多个对象共享同一个实例（避免意外修改污染数据）
 * EditInlineNwe:在Unreal编辑器的属性面板中显示 "+" 新建按钮,允许直接在父对象内部创建和编辑子对象实例
 */

UCLASS(DefaultToInstanced, EditInlineNew, Abstract, BlueprintType)
class UE_ROUGELIKEDEMO_API URLInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
	virtual void OnInstancedCreate(URLInventoryItemInstance* Instance) const;
};
