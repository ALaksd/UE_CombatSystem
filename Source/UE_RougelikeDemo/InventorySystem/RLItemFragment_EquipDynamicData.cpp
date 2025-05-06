// Fill out your copyright notice in the Description page of Project Settings.


#include "RLItemFragment_EquipDynamicData.h"

void URLItemFragment_EquipDynamicData::AddLevel() const
{
	//TODO:增加消耗资源
	CurrentLevel++;
	OnWeaponUpgrade.Broadcast(CurrentLevel);
}
