// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimationNotify/AN_OtherAbility.h"

#include "Component/CloseCombatComponent.h"
#include "Slate/SGameLayerManager.h"

void UAN_OtherAbility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation,EventReference);

	OnSkill.Broadcast(MeshComp);

	if (!MeshComp)	return;
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;
	
	
	if (UCloseCombatComponent* CloseComponent = Owner->FindComponentByClass<UCloseCombatComponent>())
	{
		CloseComponent->EnableFireRant(ActiveTime);
	}
}
