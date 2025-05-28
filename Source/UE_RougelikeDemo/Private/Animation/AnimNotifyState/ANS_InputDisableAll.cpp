// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_InputDisableAll.h"
#include <EnhancedInputSubsystems.h>
#include <Component/RL_MovementComponent.h>


void UANS_InputDisableAll::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration,EventReference);
	AActor* Owner = MeshComp->GetOwner();

	if (!Owner) return;

	URL_MovementComponent* MovementComp = Owner->FindComponentByClass<URL_MovementComponent>();
	if (MovementComp)
	{
		//如果TagsToBlock为空，则禁用全部输入，否则禁用指定的输入
		if (TagsToBlock.IsEmpty())
		{
			MovementComp->SetbAcceptInput(false);
		}
		else
		{
			MovementComp->BlockInputTag(TagsToBlock);
		}
		
	}
}

