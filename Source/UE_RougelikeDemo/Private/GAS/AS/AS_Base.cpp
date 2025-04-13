// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Base.h"
#include "Net/UnrealNetwork.h"



UAS_Base::UAS_Base()
{
	
}

void UAS_Base::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UAS_Base, Health);
	DOREPLIFETIME_CONDITION_NOTIFY(UAS_Base, Health, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAS_Base, MaxHealth, COND_None, REPNOTIFY_OnChanged);
}

void UAS_Base::OnRep_Health(const FGameplayAttributeData OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_Base, Health, OldHealth);
}

void UAS_Base::OnRep_MaxHealth(const FGameplayAttributeData OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_Base, MaxHealth, OldMaxHealth);
}



void UAS_Base::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeBaseChange(Attribute,NewValue);
}

void UAS_Base::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}
