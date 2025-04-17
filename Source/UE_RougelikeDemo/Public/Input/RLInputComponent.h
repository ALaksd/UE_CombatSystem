// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "RLInputConfig.h"
#include "RLInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLInputComponent : public UEnhancedInputComponent
{	
	GENERATED_BODY()

public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HoldFuncType>
	void BindAbilityInputAction(const URLInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HoldFuncType HoldFunc);

};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HoldFuncType>
void URLInputComponent::BindAbilityInputAction(const URLInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HoldFuncType HoldFunc)
{
	check(InputConfig);

	for(const auto& Action : InputConfig->AbilityInputActions)
	{
		if(Action.InputAction && Action.InputTage.IsValid())
		{
			if(PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTage);
			}
			
			if(ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTage);
			}
			
			if(HoldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HoldFunc, Action.InputTage);
			}
		}
	}
}
