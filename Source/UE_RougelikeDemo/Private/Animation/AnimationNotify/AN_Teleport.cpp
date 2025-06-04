// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimationNotify/AN_Teleport.h"

void UAN_Teleport::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	OnNotifyReceived.Broadcast(MeshComp, Animation);
}
