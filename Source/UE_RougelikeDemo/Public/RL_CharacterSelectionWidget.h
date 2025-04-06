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
	// ������ NativePreConstruct() ����
	virtual void NativePreConstruct() override;



public:
	//ָ���ɫѡ������ָ��
	class ARL_PlayerSpawner* CharacterSelection;
protected:
	// �󶨰�ť
	UPROPERTY(meta = (BindWidget))
	class UButton* Button1;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button2;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button3;

	//��ʼ��Ϸ��ť
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	//�����ťִ�е��¼�
	void OnButtonClicked(int32 ButtonIndex, ECharacterType CharacterType, UButton* SelectedButton);

	//���ڰ󶨵İ�ť����
	UFUNCTION()
	void OnButton1Clicked();
	UFUNCTION()
	void OnButton2Clicked();
	UFUNCTION()
	void OnButton3Clicked();

	//ѡ���ɫ
	UFUNCTION(BlueprintCallable)
	void SelectCharacter(int Index);

	//��ʼ��Ϸ
	UFUNCTION()
	void StartGame();

	//���ð�ť��ʽ
	void ResetButtonStyles();

	//���ð�ť��ʽ
	void SetSelectedButtonStyle(UButton* SelectedButton);
};
