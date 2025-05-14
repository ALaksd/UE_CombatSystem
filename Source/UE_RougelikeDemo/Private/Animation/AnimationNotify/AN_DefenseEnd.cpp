// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimationNotify/AN_DefenseEnd.h"
#include "Character/RL_BaseCharacter.h"

void UAN_DefenseEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ARL_BaseCharacter* Character = Cast<ARL_BaseCharacter>(MeshComp->GetOwner());
	if (Character)
	{
		UAbilitySystemComponent* ASC = Character->FindComponentByClass<UAbilitySystemComponent>();
		if (ASC)
		{
			ASC->RemoveLooseGameplayTag(Tag);
			UE_LOG(LogTemp, Log, TEXT("Invincibility Tag Removed"));
		}
	}
}