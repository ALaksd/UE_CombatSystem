// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/RL_UserWidget.h"

void URL_UserWidget::SetWidgetController(UObject* InWidgetConller)
{
	WidgetController = InWidgetConller;
	WidgetControllerSet();
}

