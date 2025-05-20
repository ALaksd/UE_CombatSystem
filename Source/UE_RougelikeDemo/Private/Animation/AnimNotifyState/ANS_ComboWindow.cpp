// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_ComboWindow.h"
#include "AbilitySystemBlueprintLibrary.h"


void UANS_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	//手动添加标签
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Owner, ComboTag);
}

void UANS_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	//移除标签
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Owner, ComboTag);
}
