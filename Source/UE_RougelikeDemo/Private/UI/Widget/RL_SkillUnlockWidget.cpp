// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/RL_SkillUnlockWidget.h"

#include "AbilitySystemComponent.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Data/Structs.h"
#include "GAS/ASC_Base.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RL_PlayerState.h"
#include "UI/Widget/RL_SkillSlotWidget.h"
#include "Styling/CoreStyle.h"
#include "UI/WidgetController/RL_LanternFlameController.h"

void URL_SkillUnlockWidget::AddSkillSlot(const FSkillList&  SkillInfor)
{
	SkillCount++;
	if (SkillCount%4==1)
	{
		SkillCount=1;
		//创建一行新的技能栏
		SkillSlot = CreateWidget<URL_SkillSlotWidget>(this,SkillSlotClass);
		SkillSlot->SkillSlot_1->SetIsEnabled(false);
		SkillSlot->SkillSlot_2->SetIsEnabled(false);
		SkillSlot->SkillSlot_3->SetIsEnabled(false);
		SkillSlot->SkillSlot_4->SetIsEnabled(false);
		SkillListView->AddChild(SkillSlot);

		SkillSlot->OnSkillButtonClicked.AddUObject(this,&URL_SkillUnlockWidget::OnSkillButtonClicked);
	}
	if (!SkillSlot)
	{
		//创建一行新的技能栏
		SkillSlot = CreateWidget<URL_SkillSlotWidget>(this,SkillSlotClass);
		SkillSlot->SkillSlot_1->SetIsEnabled(false);
		SkillSlot->SkillSlot_2->SetIsEnabled(false);
		SkillSlot->SkillSlot_3->SetIsEnabled(false);
		SkillSlot->SkillSlot_4->SetIsEnabled(false);
		SkillListView->AddChild(SkillSlot);

		SkillSlot->OnSkillButtonClicked.AddUObject(this,&URL_SkillUnlockWidget::OnSkillButtonClicked);
	}
	
	//修改图标
	switch (SkillCount)
	{
		case 1:
			// 存储技能
			SkillSlot->SkillSlot_1->SetIsEnabled(true);
			SkillMap.Add(SkillSlot->SkillSlot_1,SkillInfor);
			SetButtonNormalStyleImage(SkillSlot->SkillSlot_1,SkillInfor.Icon);
			
			break;
		case 2:
			SkillSlot->SkillSlot_2->SetIsEnabled(true);
			SkillMap.Add(SkillSlot->SkillSlot_2,SkillInfor);
			SetButtonNormalStyleImage(SkillSlot->SkillSlot_2,SkillInfor.Icon);
			break;
		case 3:
			SkillSlot->SkillSlot_3->SetIsEnabled(true);
			SkillMap.Add(SkillSlot->SkillSlot_3,SkillInfor);
			SetButtonNormalStyleImage(SkillSlot->SkillSlot_3,SkillInfor.Icon);
			
			break;
		case 4:
			SkillSlot->SkillSlot_4->SetIsEnabled(true);
			SkillMap.Add(SkillSlot->SkillSlot_4,SkillInfor);
			SetButtonNormalStyleImage(SkillSlot->SkillSlot_4,SkillInfor.Icon);
			
			break;
		default:break;
	}

	if (!bInit)
	{
		bInit=true;
		SkillSlot->SkillSlot_1->OnClicked.Broadcast();
	}
}

bool URL_SkillUnlockWidget::Initialize()
{
	if (Super::Initialize())
	{
		Cost->OnClicked.AddDynamic(this,&URL_SkillUnlockWidget::OnLearnClicked);
		return true;
	}
	return false;
}

void URL_SkillUnlockWidget::SetButtonNormalStyleImage(UButton* Button, UTexture2D* NewImage)
{
	if (!NewImage || !Button) return ;
	
	// 创建新的 Slate Brush 并应用贴图
	 FSlateBrush NewBrush;
	 NewBrush.SetResourceObject(NewImage);
	 NewBrush.ImageSize = FVector2D(NewImage->GetSizeX(), NewImage->GetSizeY());
	 NewBrush.DrawAs = ESlateBrushDrawType::Image;
	
	FButtonStyle NewStyle = FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button");
	NewStyle.SetNormal(NewBrush);

	
	Button->SetStyle(NewStyle);

}

void URL_SkillUnlockWidget::OnSkillButtonClicked(UButton* ButtonClicked)
{
	if (FSkillList* Skill = SkillMap.Find(ButtonClicked))
	{
		CurrentSkill = Skill;
		
		SkillName->SetText(FText::FromString(Skill->SkillName.ToString()));
		SkillStats->SetText(FText::FromString(Skill->SkillAttribute));
		SkillDescription->SetText(FText::FromString(Skill->Description));
	}
}

void URL_SkillUnlockWidget::OnLearnClicked()
{
	if (!CurrentSkill) return ;

	for (TSubclassOf<UGameplayAbility> NewAbility : CurrentSkill->GA)
	{
		if (ARL_PlayerState* PlayerState = Cast<ARL_PlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0)))
		{
			if (UASC_Base* ASC = Cast<UASC_Base>(PlayerState->GetAbilitySystemComponent()))
			{
				ASC->AddCharacterAbility(NewAbility);
			}
		}
	}
}
