// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_CharacterSelectionWidget.h"
#include "Components/Button.h"
#include "Spawner/RL_PlayerSpawner.h"
#include "Interface/RL_GameInstanceInterface.h"
#include "Kismet/GameplayStatics.h"

//void URL_CharacterSelectionWidget::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//	// 获取 PlayerController 并设置 UI 模式和显示鼠标光标
//	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
//	if (PlayerController)
//	{
//		FInputModeUIOnly InputMode;
//		InputMode.SetWidgetToFocus(this->TakeWidget());  // 设置当前 Widget 为焦点
//		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
//
//		PlayerController->SetInputMode(InputMode);
//		PlayerController->bShowMouseCursor = true;
//	}
//
//	if (Button1 && Button2 && Button3 && StartButton)
//	{
//		Button1->OnClicked.AddDynamic(this, &URL_CharacterSelectionWidget::OnButton1Clicked);
//		Button2->OnClicked.AddDynamic(this, &URL_CharacterSelectionWidget::OnButton2Clicked);
//		Button3->OnClicked.AddDynamic(this, &URL_CharacterSelectionWidget::OnButton3Clicked);
//		StartButton->OnClicked.AddDynamic(this, &URL_CharacterSelectionWidget::StartGame);
//	}
//}
//
//void URL_CharacterSelectionWidget::NativePreConstruct()
//{
//	Super::NativePreConstruct();
//}
//
//void URL_CharacterSelectionWidget::OnButtonClicked(int32 ButtonIndex, ECharacterType CharacterType, UButton* SelectedButton)
//{
//	ResetButtonStyles();
//
//	// 选择角色
//	SelectCharacter(ButtonIndex);
//
//	 // 获取游戏实例并调用接口函数
//	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
//	if (IRL_GameInstanceInterface* GameInstanceInterface = Cast<IRL_GameInstanceInterface>(GameInstance))
//	{
//		GameInstanceInterface->SetSelectCharacter(CharacterType);
//	}
//
//
//	// 设置按钮的选中状态外观
//	SetSelectedButtonStyle(SelectedButton);
//}
//
//// 这三个函数调用通用函数，并传入具体参数
//void URL_CharacterSelectionWidget::OnButton1Clicked()
//{
//	OnButtonClicked(1, ECharacterType::Warrior, Button1);
//}
//
//void URL_CharacterSelectionWidget::OnButton2Clicked()
//{
//	OnButtonClicked(2, ECharacterType::Archer, Button2);
//}
//
//void URL_CharacterSelectionWidget::OnButton3Clicked()
//{
//	OnButtonClicked(3, ECharacterType::Caster, Button3);
//}
//
//void URL_CharacterSelectionWidget::SelectCharacter(int Index)
//{
//	if (CharacterSelection)
//	{
//		CharacterSelection->SelectCharacter(Index);
//	}
//}
//
//void URL_CharacterSelectionWidget::StartGame()
//{
//	// 获取 PlayerController
//	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
//	if (PlayerController)
//	{
//		// 设置输入模式为游戏模式
//		FInputModeGameOnly InputMode;
//		PlayerController->SetInputMode(InputMode);
//
//		// 隐藏鼠标光标
//		PlayerController->bShowMouseCursor = false;
//	}
//
//	// 加载关卡
//	FName LevelName = TEXT("Combat_Practice1"); // 替换为你想加载的关卡名
//	UGameplayStatics::OpenLevel(this, LevelName);
//}
//
//void URL_CharacterSelectionWidget::ResetButtonStyles()
//{
//	if (Button1) Button1->SetColorAndOpacity(FLinearColor::White);  // 设置为默认白色
//	if (Button2) Button2->SetColorAndOpacity(FLinearColor::White);  // 设置为默认白色
//	if (Button3) Button3->SetColorAndOpacity(FLinearColor::White);  // 设置为默认白色
//}
//
//void URL_CharacterSelectionWidget::SetSelectedButtonStyle(UButton* SelectedButton)
//{
//	SelectedButton->SetColorAndOpacity(FLinearColor::Blue);
//}
