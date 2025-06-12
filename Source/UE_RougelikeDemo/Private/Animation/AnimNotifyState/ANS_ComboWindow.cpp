// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_ComboWindow.h"
#include "AbilitySystemBlueprintLibrary.h"


void UANS_ComboWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	//�ֶ����ӱ�ǩ
	UAbilitySystemBlueprintLibrary::AddLooseGameplayTags(Owner, ComboTag);
}

void UANS_ComboWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	//�Ƴ���ǩ
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(Owner, ComboTag);
}
