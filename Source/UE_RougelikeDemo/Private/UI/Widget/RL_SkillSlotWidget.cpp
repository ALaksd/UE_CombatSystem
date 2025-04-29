// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/RL_SkillSlotWidget.h"

#include "Components/Button.h"

bool URL_SkillSlotWidget::Initialize()
{
	if (Super::Initialize())
	{
		SkillSlot_1->OnClicked.AddDynamic(this,&URL_SkillSlotWidget::OnButton_1Clicked);
		SkillSlot_2->OnClicked.AddDynamic(this,&URL_SkillSlotWidget::OnButton_2Clicked);
		SkillSlot_3->OnClicked.AddDynamic(this,&URL_SkillSlotWidget::OnButton_3Clicked);
		SkillSlot_4->OnClicked.AddDynamic(this,&URL_SkillSlotWidget::OnButton_4Clicked);
		
		return true;
	}
	return false;
}

void URL_SkillSlotWidget::OnButton_1Clicked()
{
	OnSkillButtonClicked.Broadcast(SkillSlot_1);
}

void URL_SkillSlotWidget::OnButton_2Clicked()
{
	OnSkillButtonClicked.Broadcast(SkillSlot_2);
}

void URL_SkillSlotWidget::OnButton_3Clicked()
{
	OnSkillButtonClicked.Broadcast(SkillSlot_3);
}

void URL_SkillSlotWidget::OnButton_4Clicked()
{
	OnSkillButtonClicked.Broadcast(SkillSlot_4);
}
