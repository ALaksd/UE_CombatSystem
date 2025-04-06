// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_CharacterSelectionWidget.h"
#include "Components/Button.h"
#include "Spawner/RL_PlayerSpawner.h"
#include "Interface/RL_GameInstanceInterface.h"
#include "Kismet/GameplayStatics.h"

void URL_CharacterSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ��ȡ PlayerController ������ UI ģʽ����ʾ�����
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(this->TakeWidget());  // ���õ�ǰ Widget Ϊ����
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}

	if (Button1 && Button2 && Button3 && StartButton)
	{
		Button1->OnClicked.AddDynamic(this, &URL_CharacterSelectionWidget::OnButton1Clicked);
		Button2->OnClicked.AddDynamic(this, &URL_CharacterSelectionWidget::OnButton2Clicked);
		Button3->OnClicked.AddDynamic(this, &URL_CharacterSelectionWidget::OnButton3Clicked);
		StartButton->OnClicked.AddDynamic(this, &URL_CharacterSelectionWidget::StartGame);
	}
}

void URL_CharacterSelectionWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void URL_CharacterSelectionWidget::OnButtonClicked(int32 ButtonIndex, ECharacterType CharacterType, UButton* SelectedButton)
{
	ResetButtonStyles();

	// ѡ���ɫ
	SelectCharacter(ButtonIndex);

	 // ��ȡ��Ϸʵ�������ýӿں���
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (IRL_GameInstanceInterface* GameInstanceInterface = Cast<IRL_GameInstanceInterface>(GameInstance))
	{
		GameInstanceInterface->SetSelectCharacter(CharacterType);
	}


	// ���ð�ť��ѡ��״̬���
	SetSelectedButtonStyle(SelectedButton);
}

// ��������������ͨ�ú�����������������
void URL_CharacterSelectionWidget::OnButton1Clicked()
{
	OnButtonClicked(1, ECharacterType::Warrior, Button1);
}

void URL_CharacterSelectionWidget::OnButton2Clicked()
{
	OnButtonClicked(2, ECharacterType::Archer, Button2);
}

void URL_CharacterSelectionWidget::OnButton3Clicked()
{
	OnButtonClicked(3, ECharacterType::Caster, Button3);
}

void URL_CharacterSelectionWidget::SelectCharacter(int Index)
{
	if (CharacterSelection)
	{
		CharacterSelection->SelectCharacter(Index);
	}
}

void URL_CharacterSelectionWidget::StartGame()
{
	// ��ȡ PlayerController
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		// ��������ģʽΪ��Ϸģʽ
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);

		// ���������
		PlayerController->bShowMouseCursor = false;
	}

	// ���عؿ�
	FName LevelName = TEXT("Combat_Practice1"); // �滻Ϊ������صĹؿ���
	UGameplayStatics::OpenLevel(this, LevelName);
}

void URL_CharacterSelectionWidget::ResetButtonStyles()
{
	if (Button1) Button1->SetColorAndOpacity(FLinearColor::White);  // ����ΪĬ�ϰ�ɫ
	if (Button2) Button2->SetColorAndOpacity(FLinearColor::White);  // ����ΪĬ�ϰ�ɫ
	if (Button3) Button3->SetColorAndOpacity(FLinearColor::White);  // ����ΪĬ�ϰ�ɫ
}

void URL_CharacterSelectionWidget::SetSelectedButtonStyle(UButton* SelectedButton)
{
	SelectedButton->SetColorAndOpacity(FLinearColor::Blue);
}
