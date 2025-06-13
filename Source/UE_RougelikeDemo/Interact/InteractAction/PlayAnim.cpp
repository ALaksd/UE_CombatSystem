// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayAnim.h"

void UPlayAnim::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (SkeletalToAnimMap.IsEmpty())
	{
		for (const TPair<USkeletalMeshComponent*, UAnimMontage*>& Pair : SkeletalToMontageMap)
		{
			USkeletalMeshComponent* Mesh = Pair.Key;
			UAnimMontage* Anim = Pair.Value;
			UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
			AnimInstance->Montage_Play(Anim, 1.0f);
		}
	}
	else
	{
		for (const TPair<USkeletalMeshComponent*, UAnimSequence*>& Pair : SkeletalToAnimMap)
		{
			USkeletalMeshComponent* Mesh = Pair.Key;
			UAnimSequence* Anim = Pair.Value;
			Mesh->PlayAnimation(Anim, false);
		}
	}
}
