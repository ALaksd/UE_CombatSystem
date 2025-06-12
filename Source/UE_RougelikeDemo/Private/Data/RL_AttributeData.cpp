// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/RL_AttributeData.h"

FRLAttributeInfo URL_AttributeData::FindAttributeInfoFromTag(const FGameplayTag& AttributeTag) const
{
	for (const FRLAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}


	return FRLAttributeInfo();
}
