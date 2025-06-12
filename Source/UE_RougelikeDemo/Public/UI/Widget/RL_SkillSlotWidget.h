// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/RL_UserWidget.h"
#include "RL_SkillSlotWidget.generated.h"


class URL_SkillUnlockWidget;
class UOverlay;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillButtonCliced,UButton*,Button);

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_SkillSlotWidget : public URL_UserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	URL_SkillUnlockWidget* Parent;
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetVisible(int32 Num);

	UFUNCTION(BlueprintImplementableEvent)
	UButton* SetButtonBrushMy(UTexture2D* Icon);
	
	UPROPERTY(BlueprintCallable,BlueprintAssignable)
	FOnSkillButtonCliced OnSkillButtonCliced;

};
