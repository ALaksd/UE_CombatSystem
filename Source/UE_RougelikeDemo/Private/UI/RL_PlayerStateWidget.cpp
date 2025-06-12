// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RL_PlayerStateWidget.h"

void URL_PlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();
	AActor* Owner = GetOwningPlayerPawn();
	if (Owner)
	{
		URL_RowCompoment* BowComponent = Owner->FindComponentByClass<URL_RowCompoment>();
		if (BowComponent)
		{
			// 绑定瞄准开始和结束的事件到显示/隐藏准星的方法
			BowComponent->OnAimBegin.AddUObject(this, &URL_PlayerStateWidget::ShowCrossHair);
			BowComponent->OnAimEnd.AddUObject(this, &URL_PlayerStateWidget::HideCrossHair);

		}
	}
}

void URL_PlayerStateWidget::ShowCrossHair()
{
	if (CrossHairWidget)
	{
		CrossHairWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void URL_PlayerStateWidget::HideCrossHair()
{
	if (CrossHairWidget)
	{
		CrossHairWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
