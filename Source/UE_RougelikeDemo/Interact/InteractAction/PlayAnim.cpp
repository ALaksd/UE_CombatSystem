// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayAnim.h"

#include "InteractableActor/Interactable_LanternFlame.h"

void UPlayAnim::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (Cast<AInteractable_LanternFlame>(InteractedTarget))
	{
		for (const TPair<USkeletalMeshComponent*, UAnimSequence*>& Pair : Cast<AInteractable_LanternFlame>(InteractedTarget)->SkeletalToAnimMap)
		{
			USkeletalMeshComponent* Mesh = Pair.Key;
			UAnimSequence* Anim = Pair.Value;
			Mesh->PlayAnimation(Anim, false);
		}
	}
}
