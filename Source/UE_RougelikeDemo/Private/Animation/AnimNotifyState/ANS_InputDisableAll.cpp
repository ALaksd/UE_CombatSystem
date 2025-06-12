// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_InputDisableAll.h"
#include <EnhancedInputSubsystems.h>
#include <Component/RL_MovementComponent.h>

#include "GameplayTagsManager.h"


void UANS_InputDisableAll::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration,EventReference);
	AActor* Owner = MeshComp->GetOwner();

	if (!Owner) return;

	URL_MovementComponent* MovementComp = Owner->FindComponentByClass<URL_MovementComponent>();
	if (MovementComp)
	{

		// 默认禁用所有输入,如果TagsUnBlock不为空则排除这部分Tag
		// 获取根标签 InputTag
		FGameplayTag RootTag = UGameplayTagsManager::Get().RequestGameplayTag(FName("InputTag"), false);
		FGameplayTagContainer InputTags = UGameplayTagsManager::Get().RequestGameplayTagChildren(RootTag);
		
		if (!TagsUnBlock.IsEmpty())
		{
			InputTags.RemoveTags(TagsUnBlock);
			
		}
		MovementComp->BlockInputTag(InputTags);

	}
}

