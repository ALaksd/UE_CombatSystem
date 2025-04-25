// Fill out your copyright notice in the Description page of Project Settings.


#include "RLItemDefinition_Equip.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLInventoryFragment_Equipment.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_PhysicalDisplay.h"

URLItemDefinition_Equip::URLItemDefinition_Equip(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	Fragments.Add(CreateDefaultSubobject<URLInventoryFragment_Equipment>("EquipmentFragment"));
	Fragments.Add(CreateDefaultSubobject<URLItemFragment_PhysicalDisplay>("PhysicalDisplayFragment"));
}
