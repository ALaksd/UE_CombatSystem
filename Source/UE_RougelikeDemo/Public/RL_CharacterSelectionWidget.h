// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RL_CharacterEunm.h"
#include "RL_CharacterSelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_CharacterSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	// 或者用 NativePreConstruct() 函数
	virtual void NativePreConstruct() override;



public:
	//指向角色选择器的指针
	class ARL_PlayerSpawner* CharacterSelection;
protected:
	// 绑定按钮
	UPROPERTY(meta = (BindWidget))
	class UButton* Button1;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button2;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button3;

	//开始游戏按钮
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	//点击按钮执行的事件
	void OnButtonClicked(int32 ButtonIndex, ECharacterType CharacterType, UButton* SelectedButton);

	//用于绑定的按钮函数
	UFUNCTION()
	void OnButton1Clicked();
	UFUNCTION()
	void OnButton2Clicked();
	UFUNCTION()
	void OnButton3Clicked();

	//选择角色
	UFUNCTION(BlueprintCallable)
	void SelectCharacter(int Index);

	//开始游戏
	UFUNCTION()
	void StartGame();

	//重置按钮样式
	void ResetButtonStyles();

	//设置按钮样式
	void SetSelectedButtonStyle(UButton* SelectedButton);
};
