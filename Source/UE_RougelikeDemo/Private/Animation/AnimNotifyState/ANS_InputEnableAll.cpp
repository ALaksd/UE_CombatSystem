// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_InputEnableAll.h"
#include <EnhancedInputSubsystems.h>
#include <Component/RL_MovementComponent.h>

void UANS_InputEnableAll::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration ,const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration,EventReference);
	AActor* Owner = MeshComp->GetOwner();

	if (!Owner) return;

	if (URL_MovementComponent* MovementComp = Owner->FindComponentByClass<URL_MovementComponent>())
	{
		MovementComp->SetbAcceptInput(true);
		MovementComp->UnblockInputTag();
	}
}
