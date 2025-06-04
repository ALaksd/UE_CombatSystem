// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_RougelikeDemo/InventorySystem/Definition/RLItemDefinition_Skill.h"

#include "UE_RougelikeDemo/InventorySystem/Fragments/RLInventoryFragment_Equipment.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_UI.h"


URLItemDefinition_Skill::URLItemDefinition_Skill(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	Fragments.Add(CreateDefaultSubobject<URLInventoryFragment_Equipment>("EquipmentFragment"));
	Fragments.Add(CreateDefaultSubobject<URLItemFragment_UI>("UIFragment"));
}
