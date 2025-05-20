// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Component/RL_RowCompoment.h"
#include "Components/Image.h"
#include "RL_CorssHairUI.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_CorssHairUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UImage* OuterCircle;

	UPROPERTY(BlueprintReadOnly)
	float CircleScale;
	//ËõÐ¡×¼ÐÇ
	UFUNCTION()
	void ShortCrossHair(float Pulltime,float MaxPullTime);

	//»Ö¸´×¼ÐÇ
	UFUNCTION()
	void ResetCrossHair();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayScaleAnimation();
	UFUNCTION(BlueprintImplementableEvent)
	void ResetScaleAnimation();
};
