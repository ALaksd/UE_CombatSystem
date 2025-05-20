// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/RL_LanternFlameController.h"
#include <System/RL_SavePointSubsystem.h>

void URL_LanternFlameController::Initialize(UDataTable* SkillList_T)
{
	SkillList = SkillList_T;
}

UDataTable* URL_LanternFlameController::GetSkillList()
{
	return SkillList;
}
