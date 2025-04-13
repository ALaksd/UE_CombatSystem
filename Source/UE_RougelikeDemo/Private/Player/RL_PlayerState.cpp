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
