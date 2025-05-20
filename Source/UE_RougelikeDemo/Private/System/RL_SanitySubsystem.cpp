// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RL_SanitySubsystem.h"

void URL_SanitySubsystem::ReduceSanity(float Sanity)
{
	CurrentSanity -= Sanity;
	if (CurrentSanity <= 0)
	{
		CurrentSanity=0;
	}

	if (GetSanityPercent() <= 0.5)
	{
		SanityState=E_SanityState::Chaotic;
	}
	OnSanityStateChanged.Broadcast(GetSanityState(), CurrentSanity);

}

void URL_SanitySubsystem::RestoreSanity(float Amount)
{
	CurrentSanity += Amount;
	if (CurrentSanity>MaxSanity)
		CurrentSanity=MaxSanity;

	if (GetSanityPercent() > 0.5)
	{
		SanityState=E_SanityState::Sane;
	}
	OnSanityStateChanged.Broadcast(GetSanityState(), CurrentSanity);

}

void URL_SanitySubsystem::ReduceSanityOverTime()
{
	ReduceSanity(DeltaAmount);
}

void URL_SanitySubsystem::RestoreSanityToMax()
{
	CurrentSanity = MaxSanity;
	SanityState=E_SanityState::Sane;
	OnSanityStateChanged.Broadcast(GetSanityState(), CurrentSanity);
}

void URL_SanitySubsystem::SetCombatState(bool bInCombat)
{
	if (bIsInCombat != bInCombat)
	{
		bIsInCombat = bInCombat;
		if (bIsInCombat)
		{
			GetWorld()->GetTimerManager().SetTimer(
				AutoReduceSanityTimer,
				this,
				&URL_SanitySubsystem::ReduceSanityOverTime,
				Interval,
				true
			);
		}
		else
		{
			GetWorld()->GetTimerManager().ClearTimer(AutoReduceSanityTimer);
		}
	}

}

void URL_SanitySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	SanityState=E_SanityState::Sane;
	OnSanityStateChanged.Broadcast(GetSanityState(), CurrentSanity);
	MaxSanity=100;
	CurrentSanity=MaxSanity;
}
