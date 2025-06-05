// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimationNotify/AN_DefenseEnd.h"
#include "Character/RL_BaseCharacter.h"
#include <AbilitySystemBlueprintLibrary.h>

void UAN_DefenseEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(Tag);
		ASC->SetTagMapCount(Tag, 0);
	}
}