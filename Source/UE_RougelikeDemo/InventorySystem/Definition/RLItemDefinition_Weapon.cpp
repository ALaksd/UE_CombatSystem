// Fill out your copyright notice in the Description page of Project Settings.


#include "RLItemDefinition_Weapon.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLInventoryFragment_Equipment.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_Pickup.h"
#include <UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_Attached.h>
#include <UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_UI.h>
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_WeaponLevelData.h"

URLItemDefinition_Weapon::URLItemDefinition_Weapon(const FObjectInitializer& ObjectInitializer)
{
	Fragments.Add(CreateDefaultSubobject<URLInventoryFragment_Equipment>("EquipmentFragment"));
	Fragments.Add(CreateDefaultSubobject<URLItemFragment_Pickup>("PhysicalDisplayFragment"));
	Fragments.Add(CreateDefaultSubobject<URLItemFragment_Attached>("AttachedFragment"));
	Fragments.Add(CreateDefaultSubobject<URLItemFragment_UI>("UIDisplayFragment"));
	Fragments.Add(CreateDefaultSubobject<URLItemFragment_WeaponLevelData>("WeaponLevelData"));
}
