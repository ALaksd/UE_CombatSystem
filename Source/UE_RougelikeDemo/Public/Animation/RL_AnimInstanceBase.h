// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/RL_CharacterAimInterface.h"
#include "RL_AnimInstanceBase.generated.h"

UENUM(BlueprintType)
enum class EDirectionType : uint8
{
	ForwardDic,
	BackwardDic,
	LeftDic,
	RightDic,
};
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_AnimInstanceBase : public UAnimInstance,public IRL_CharacterAimInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	EDirectionType DicType;
};
