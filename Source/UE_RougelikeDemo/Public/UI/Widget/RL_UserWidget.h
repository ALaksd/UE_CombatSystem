// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RL_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_UserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetConller);

	//控制器（唯一）
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	//设置好控制器后执行调试或其他功能
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
