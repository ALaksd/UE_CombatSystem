// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/RL_UserWidget.h"
#include "RL_SkillSlotWidget.generated.h"


class UButton;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkillButtonCliced,UButton*)

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_SkillSlotWidget : public URL_UserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget),Category="Components")
	TObjectPtr<UButton> SkillSlot_1;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget),Category="Components")
	TObjectPtr<UButton> SkillSlot_2;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget),Category="Components")
	TObjectPtr<UButton> SkillSlot_3;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidget),Category="Components")
	TObjectPtr<UButton> SkillSlot_4;

	FOnSkillButtonCliced OnSkillButtonClicked;

private:
	virtual bool Initialize() override;
	UFUNCTION()
	void OnButton_1Clicked();
	UFUNCTION()
	void OnButton_2Clicked();
	UFUNCTION()
	void OnButton_3Clicked();
	UFUNCTION()
	void OnButton_4Clicked();
};
