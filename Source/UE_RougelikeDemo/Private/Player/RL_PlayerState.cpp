// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RL_PlayerState.h"
#include <GAS/ASC_Base.h>

ARL_PlayerState::ARL_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UASC_Base>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAS_Player>(TEXT("AttributeSet"));
	NetUpdateFrequency = 100;
}

void ARL_PlayerState::SetSoul(int32 NewSoul)
{
	Soul = NewSoul;
	OnSoulChanged.Broadcast(Soul);
}

void ARL_PlayerState::AddSoul(int32 NewSoul)
{
	Soul += NewSoul;
	OnSoulChanged.Broadcast(Soul);
}

void ARL_PlayerState::AddLevel()
{
	//需要确保已有的灵魂大于所需灵魂
	int32 Need = GetLevelRequirement(Level);
	AddSoul(-Need);
	Level += 1;
	OnLevelChanged.Broadcast(Level);
}

void ARL_PlayerState::AddSpellPoints(int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
}

int32 ARL_PlayerState::GetLevelRequirement(int32 InLevel)
{
	FName RowName = FName(*FString::Printf(TEXT("%d"), InLevel + 1));

	const FLevelUpInfo* Row = LevelUpInfo->FindRow<FLevelUpInfo>(RowName,TEXT("Lookup Level"));
	if (Row)
	{
		return Row->LevelUpRequirement;
	}

	return 0;
}

void ARL_PlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//��ȡ���ݱ��������������

}

void ARL_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARL_PlayerState::OnRep_Health(const FGameplayAttributeData& OldHealth)
{

}

void ARL_PlayerState::OnRep_Endurance(const FGameplayAttributeData& OldHealth)
{

}

void ARL_PlayerState::OnRep_Momentum(const FGameplayAttributeData& OldHealth)
{

}
