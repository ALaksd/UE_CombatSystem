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
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_Skill.h"

void URL_SkillUnlockWidget::AddSkillSlot(const FRL_Skill&  SkillInfor)
{
	SkillCount++;
	if (SkillCount%4==1)
	{
		SkillCount=1;
		//创建一行新的技能栏
		SkillSlot = CreateWidget<URL_SkillSlotWidget>(this,SkillSlotClass);
		SkillSlot->Parent=this;
		SkillListView->AddChild(SkillSlot);

		SkillSlot->OnSkillButtonCliced.AddDynamic(this,&URL_SkillUnlockWidget::OnSkillButtonClicked);
	}
	if (!SkillSlot)
	{
		//创建一行新的技能栏
		SkillSlot = CreateWidget<URL_SkillSlotWidget>(this,SkillSlotClass);
		SkillSlot->Parent=this;
		SkillListView->AddChild(SkillSlot);

		SkillSlot->OnSkillButtonCliced.AddDynamic(this,&URL_SkillUnlockWidget::OnSkillButtonClicked);
	}
	
	SkillSlot->SetVisible(SkillCount);
	// UButton* Button = SkillSlot->SetButtonBrushMy(SkillInfor.Skill->FindFragmentByClass<URLItemFragment_Skill>(URLItemFragment_Skill::StaticClass())->Icon);
	UButton* Button = SkillSlot->SetButtonBrushMy(SkillInfor.Icon);

	// 将按钮与技能绑定
	if (Button)
		SkillMap.Add(Button,SkillInfor);

	// 初始化解锁界面技能图标
	if (!bInit)
	{
		bInit=true;
		Button->OnClicked;
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
	if (FRL_Skill* Skill = SkillMap.Find(ButtonClicked))
	{
		//CurrentSkill = Skill->Skill->FindFragmentByClass<URLItemFragment_Skill>(URLItemFragment_Skill::StaticClass());
		CurrentSkill = Skill;
		
		SkillName->SetText(FText::FromString(CurrentSkill->SkillName.ToString()));
		SkillStats->SetText(FText::FromString(CurrentSkill->SkillAttribute));
		SkillDescription->SetText(FText::FromString(CurrentSkill->Description));
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
				// TODO:将技能装入技能背包
				ASC->AddCharacterAbility(NewAbility);
			}
		}
	}
}
