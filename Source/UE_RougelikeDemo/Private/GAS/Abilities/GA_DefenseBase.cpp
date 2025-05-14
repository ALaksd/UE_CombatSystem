// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/Abilities/GA_DefenseBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Component/CloseCombatComponent.h"

UGA_DefenseBase::UGA_DefenseBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_DefenseBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		if (SwordMontage && ActorInfo->AvatarActor->GetComponentByClass<UCloseCombatComponent>()->GetCurrentWeapon()->Type == WeaponType::Sword)
		{
			UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				SwordMontage,
				1.0f,
				NAME_None,
				false
			);

			if (PlayMontageTask)
			{
				PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_DefenseBase::OnMontageCompleted);
				PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_DefenseBase::OnMontageCompleted);
				PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_DefenseBase::OnMontageCompleted);
				PlayMontageTask->ReadyForActivation();
			}
		}
		else if (GreatSwordMontage && ActorInfo->AvatarActor->GetComponentByClass<UCloseCombatComponent>()->GetCurrentWeapon()->Type == WeaponType::GreatSword)
		{
			UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				GreatSwordMontage,
				1.0f,
				NAME_None,
				false
			);

			if (PlayMontageTask)
			{
				PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_DefenseBase::OnMontageCompleted);
				PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_DefenseBase::OnMontageCompleted);
				PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_DefenseBase::OnMontageCompleted);
				PlayMontageTask->ReadyForActivation();
			}
		}
		else
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
		}
	}
}

void UGA_DefenseBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_DefenseBase::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
