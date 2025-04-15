// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_PreInputManage.h"
#include "Component/RL_InputBufferComponent.h"
#include "GameplayTagContainer.h"

void UANS_PreInputManage::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;
	URL_InputBufferComponent* Buffer = Owner->FindComponentByClass<URL_InputBufferComponent>();
	if (Buffer)
	{
		Buffer->SetbAcceptingBufferedInput(true);
	}
}

void UANS_PreInputManage::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	URL_InputBufferComponent* Buffer = Owner->FindComponentByClass<URL_InputBufferComponent>();
	if (Buffer)
	{
		Buffer->SetbAcceptingBufferedInput(false);
		FGameplayTag InputTag = Buffer->ConsumeBufferedInput();
		UE_LOG(LogTemp, Log, TEXT("Excute InputTag: %s"), *InputTag.ToString());
		// ÷¥–– ‰»Î
		//TriggerActionByTag(InputTag);
	}
}

