// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/RL_WidgetController.h"
#include "System/RL_SavePointSubsystem.h"
#include "RL_LanternFlameController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UE_ROUGELIKEDEMO_API URL_LanternFlameController : public URL_WidgetController
{
	GENERATED_BODY()

private:
	//技能表的数据
	UPROPERTY()
	TObjectPtr<UDataTable> SkillList;

	//当前传送点数据
	FSavePointData CurrentSavePointData;

public:
	void Initialize(UDataTable* SkillList_T);

	UFUNCTION(BlueprintCallable)
	UDataTable* GetSkillList();
};
