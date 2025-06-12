// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_WeaponLevelData.h"

FWeaponLevelData URLItemFragment_WeaponLevelData::GetWeaponLevelData(int32 Level) const
{
	if (!WeaponLevelDataTable)
		return FWeaponLevelData();

	const FWeaponLevelData* DataPtr = WeaponLevelDataTable->FindRow<FWeaponLevelData>(
		FName(FString::FromInt(Level)),
		TEXT("Weapon Level Data"),
		true // 启用缺失数据警告
	);

	return *DataPtr;
}
