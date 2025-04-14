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

	//��������Ψһ��
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

	//���úÿ�������ִ�е��Ի���������
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
