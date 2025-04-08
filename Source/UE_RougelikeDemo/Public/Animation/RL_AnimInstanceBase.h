// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/RL_CharacterAimInterface.h"
#include "RL_AnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_AnimInstanceBase : public UAnimInstance,public IRL_CharacterAimInterface
{
	GENERATED_BODY()
	
};
