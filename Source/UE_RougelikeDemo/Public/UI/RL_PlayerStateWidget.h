// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Component/RL_RowCompoment.h"
#include "UI/RL_CorssHairUI.h"
#include "RL_PlayerStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_PlayerStateWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	virtual void NativeConstruct() override;

	// 绑定 CrossHairUI 的子控件
	UPROPERTY(meta = (BindWidget))
	URL_CorssHairUI* CrossHairWidget;
	
	//显示准星
	UFUNCTION()
	void ShowCrossHair();

	//隐藏准星
	UFUNCTION()
	void HideCrossHair();
};
