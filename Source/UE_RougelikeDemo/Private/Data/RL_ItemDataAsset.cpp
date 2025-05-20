// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/RL_ItemDataAsset.h"

const FItemDataInfo& URL_ItemDataAsset::FindItemDataInfoByTag(const FGameplayTag& Tag)
{
	const FItemDataInfo* Row = nullptr;
	//循环遍历ItemDataAsset，找到正确的FItemDataInfoMappingTag
	for (const auto& Pair : ItemDataAsset)
	{
		if (!Tag.MatchesTag(Pair.Tag)) { continue; }

		//循环遍历FItemDataInfo数据表，找到正确的FItemDataInfo行
		Row = FindItemDataInfoRowByTag<FItemDataInfo>(Pair.ItemDataInfo, Tag);
		break;
	}
	check(Row);
	return *Row;
}
