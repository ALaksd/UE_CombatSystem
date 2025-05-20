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

	// �� CrossHairUI ���ӿؼ�
	UPROPERTY(meta = (BindWidget))
	URL_CorssHairUI* CrossHairWidget;
	
	//��ʾ׼��
	UFUNCTION()
	void ShowCrossHair();

	//����׼��
	UFUNCTION()
	void HideCrossHair();
};
