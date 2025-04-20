// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_AttackTest.h"

#include "Character/RL_BaseCharacter.h"
#include "Component/CloseCombatComponent.h"

void UANS_AttackTest::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	Caller = Cast<ACharacter>(MeshComp->GetOwner());
	if (Caller)
		if (UCloseCombatComponent* CloseCombatComponent = Caller->FindComponentByClass<UCloseCombatComponent>())
		CloseCombatComponent->StartCombat(DamageSpecHandle);
}

void UANS_AttackTest::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (Caller)
		if (UCloseCombatComponent* CloseCombatComponent = Caller->FindComponentByClass<UCloseCombatComponent>())
			CloseCombatComponent->EndCombat();

}
