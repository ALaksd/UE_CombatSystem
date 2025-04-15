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
	template<class UserClass,typename PressedFuncType,typename ReleasedFuncType,typename HeldFuncType>
	void BindAbilityInputAction(const URLInputConfig* InputConfig,UserClass* Object,PressedFuncType PressedFunc,ReleasedFuncType ReleaceFunc,HeldFuncType HeledFunc);
	
};


template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void URLInputComponent::BindAbilityInputAction(const URLInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleaceFunc, HeldFuncType HeledFunc)
{
	check(InputConfig);

	for (const auto& Action:InputConfig->AbilityInputActions)
	{
		if (Action.InputAction&&Action.GameplayTage.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started,Object,PressedFunc,Action.GameplayTage);
			}
			if (ReleaceFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Completed,Object,ReleaceFunc,Action.GameplayTage);
			}
			if (HeledFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Triggered,Object,HeledFunc,Action.GameplayTage);
			}
		}
	}
}
