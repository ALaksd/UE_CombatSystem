// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimationNotify/AN_OtherAbility.h"

void UAN_OtherAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation,EventReference);

	OnSkill.Broadcast(MeshComp);
}
