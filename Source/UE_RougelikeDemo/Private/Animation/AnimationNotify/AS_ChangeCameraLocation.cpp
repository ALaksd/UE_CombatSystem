// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimationNotify/AS_ChangeCameraLocation.h"

#include "Character/RL_BaseCharacter.h"

void UAS_ChangeCameraLocation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (!MeshComp) return;
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)	return ;
	ARL_BaseCharacter* Character = Cast<ARL_BaseCharacter>(Owner);
	if (!Character)	return ;

	Character->SetCameraLocation(CameraLocation);
	
}
