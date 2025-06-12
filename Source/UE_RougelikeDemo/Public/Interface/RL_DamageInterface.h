// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RL_DamageInterface.generated.h"


struct FGameplayEffectSpecHandle;


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URL_DamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  需要处理伤害的Actor继承,外部Cast后调用,由各自Actor实现具体逻辑
 */
class UE_ROUGELIKEDEMO_API IRL_DamageInterface
{
	GENERATED_BODY()

public:
	virtual void TakeDamage(const FGameplayEffectSpecHandle& DamageHandle) const =0;
};
