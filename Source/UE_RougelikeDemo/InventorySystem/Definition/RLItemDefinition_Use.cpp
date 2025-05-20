// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_RougelikeDemo/InventorySystem/Definition/RLItemDefinition_Use.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_Pickup.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_UI.h"

URLItemDefinition_Use::URLItemDefinition_Use(const FObjectInitializer& ObjectInitializer)
{
	Fragments.Add(CreateDefaultSubobject<URLItemFragment_Pickup>("PhysicalDisplayFragment"));
	Fragments.Add(CreateDefaultSubobject<URLItemFragment_UI>("UIDisplayFragment"));
}
