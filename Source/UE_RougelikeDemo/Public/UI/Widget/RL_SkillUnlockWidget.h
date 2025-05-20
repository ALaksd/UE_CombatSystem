// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/RL_UserWidget.h"
#include "Data/Structs.h"

#include "RL_SkillUnlockWidget.generated.h"

class UButton;
class UScrollBox;
class UTextBlock;
class URL_SkillSlotWidget;

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_SkillUnlockWidget : public URL_UserWidget
{
	GENERATED_BODY()

public:
	/***--------------------UI控件---------------------***/
	//技能名字
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget),Category="Components")
	TObjectPtr<UTextBlock> SkillName;
	//技能属性
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget),Category="Components")
	TObjectPtr<UTextBlock> SkillStats;
	//技能描述
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget),Category="Components")
	TObjectPtr<UTextBlock> SkillDescription;
	//技能栏
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget),Category="Components")
	TObjectPtr<UScrollBox> SkillListView;
	// 学习按钮
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget),Category="Components")
	TObjectPtr<UButton> Cost;
	/***--------------------UI控件---------------------***/

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	TSubclassOf<URL_SkillSlotWidget> SkillSlotClass;
	


	UFUNCTION(BlueprintCallable)
	void AddSkillSlot(const FSkillList& SkillInfor);

	virtual bool Initialize() override;
private:
	int32 SkillCount = 0;
	UPROPERTY()
	TObjectPtr<URL_SkillSlotWidget> SkillSlot;

	void SetButtonNormalStyleImage(UButton* Button, UTexture2D* NewImage);
	//存储所有的按键对应的技能,用于后续技能显示
	TMap<TObjectPtr<UButton>,FSkillList> SkillMap;

	UFUNCTION()
	void OnSkillButtonClicked(UButton* ButtonClicked);

	FSkillList* CurrentSkill;
	UFUNCTION()
	void OnLearnClicked();

	// 用于初始化第一个技能
	bool bInit=false;
};
