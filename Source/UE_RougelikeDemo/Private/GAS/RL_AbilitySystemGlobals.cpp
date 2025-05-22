// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RL_AbilitySystemGlobals.h"
#include "GAS\RL_CustomGameplayEffectContext.h"

FGameplayEffectContext* URL_AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRLGameplayEffectContext();
}
