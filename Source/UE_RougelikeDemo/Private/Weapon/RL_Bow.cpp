// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RL_Bow.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void ARL_Bow::PullBowStart()
{
	BowState = EBowState::Draw;
	if (BowDrawSound && GetWorld())
	{
		SoundToPlay = UGameplayStatics::SpawnSoundAtLocation(this,BowDrawSound, GetActorLocation());
	}
}

void ARL_Bow::PullBowEnd()
{
	BowState = EBowState::Idle;
	if (SoundToPlay)
	{
		SoundToPlay->Stop();
	}
}
