// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Base.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UAS_Base::UAS_Base()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
}

void UAS_Base::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DATETIMEPICKER(UAS_Base, Health);
	DOREPLIFETIME_CONDITION_NOTIFY(UAS_Base, Health, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAS_Base, MaxHealth, COND_None, REPNOTIFY_OnChanged);
}

void UAS_Base::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_Base, Health, OldHealth);
}

void UAS_Base::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_Base, MaxHealth, OldMaxHealth);
}

void UAS_Base::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	//限制血量不能超过最大血量
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxHealth());
	}
}


void UAS_Base::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeBaseChange(Attribute,NewValue);
}

void UAS_Base::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	Data.EvaluatedData.Attribute = GetHealthAttribute();
	
}
