// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_AttackDecision.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Component/CloseCombatComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/RL_Sword.h"

void UANS_AttackDecision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ACharacter* Player = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (UCloseCombatComponent* CloseCombatComponent = Player->FindComponentByClass<UCloseCombatComponent>())
		{
			//向武器应用增幅
			if (UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CloseCombatComponent->GetCurrentWeapon()))
			{
				FGameplayEffectSpecHandle Handle = SourceASC->MakeOutgoingSpec(GameEffect,1,SourceASC->MakeEffectContext());
				SourceASC->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
				
				CloseCombatComponent->StartCombat();
			}
		}
	}
}

void UANS_AttackDecision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (ACharacter* Player = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		if (UCloseCombatComponent* CloseCombatComponent = Player->FindComponentByClass<UCloseCombatComponent>())
		{
			//向武器应用增幅
			if (UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CloseCombatComponent->GetCurrentWeapon()))
			{
				FGameplayEffectSpecHandle Handle = SourceASC->MakeOutgoingSpec(GameEffect,0,SourceASC->MakeEffectContext());
				SourceASC->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());

				CloseCombatComponent->EndCombat();
			}

		}
		
	}
	
}
