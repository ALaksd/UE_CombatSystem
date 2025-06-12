// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimationNotify/AN_DefenseStart.h"
#include "Character/RL_BaseCharacter.h"
#include <AbilitySystemBlueprintLibrary.h>

void UAN_DefenseStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (ASC)
	{
		ASC->AddLooseGameplayTag(Tag);
		ASC->SetTagMapCount(Tag, 1);
	}
}