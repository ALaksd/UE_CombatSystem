// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimationNotify/AN_DefenseStart.h"
#include "Character/RL_BaseCharacter.h"

void UAN_DefenseStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ARL_BaseCharacter* Character = Cast<ARL_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		UAbilitySystemComponent* ASC = Character->GetPlayerState()->FindComponentByClass<UAbilitySystemComponent>();
		if (ASC)
		{
			ASC->AddLooseGameplayTag(Tag);
			ASC->SetTagMapCount(Tag, 1);
			UE_LOG(LogTemp, Log, TEXT("Invincibility Tag Added"));
		}
	}
}