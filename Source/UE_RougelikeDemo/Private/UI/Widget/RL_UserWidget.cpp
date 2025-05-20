// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/RL_UserWidget.h"

void URL_UserWidget::SetWidgetController(UObject* InWidgetConller, URL_UserWidget* InOnwerWidget)
{
	WidgetController = InWidgetConller;
	OnwerWidget = InOnwerWidget;
	WidgetControllerSet();
}

void URL_UserWidget::ApplyInitParams_Implementation(const FWidgetInitParams& InitParams)
{
	CurrentParams = InitParams; // 存储参数
}
