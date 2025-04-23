// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RLInventoryItemFragment.generated.h"

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
