// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/RLInputConfig.h"

const UInputAction* URLInputConfig::FindAbilityInputAction(const FGameplayTag Tag, bool bLogNotFound) const
{
	for (const auto& var : AbilityInputActions)
	{
		if (var.InputAction && Tag == var.InputTage)
		{
			return var.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp,Warning,TEXT("Not Found AbilityInputAction for InputTag [%ls],on InputConfig[%s]"),*Tag.ToString(),*GetNameSafe(this));
	}

	return nullptr;
}
