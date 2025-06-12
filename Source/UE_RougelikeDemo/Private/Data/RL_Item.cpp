// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/RL_Item.h"

const FItemData& URL_Item::UpdateAmount(const int32& Magnitude)
{
	ItemData.Amount += Magnitude;
	ItemData.Amount = FMath::Clamp(ItemData.Amount, 0, ItemData.MaxStack);

	return ItemData;
}
