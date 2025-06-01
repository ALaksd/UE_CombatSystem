// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_Jump.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Character/RL_BaseCharacter.h"

UGA_Jump::UGA_Jump()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Jump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		Cast<ARL_BaseCharacter>(ActorInfo->AvatarActor)->Jump();

		if (UAnimMontage* MontageToPlay = JumpMontage)
		{
			UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				MontageToPlay,
				1.0f,
				NAME_None,
				false
			);

			if (Task)
			{
				Task->OnCompleted.AddDynamic(this, &UGA_Jump::OnMontageCompleted);
				Task->OnCancelled.AddDynamic(this, &UGA_Jump::OnMontageCompleted);
				Task->OnBlendOut.AddDynamic(this, &UGA_Jump::OnMontageCompleted);
				Task->OnInterrupted.AddDynamic(this, &UGA_Jump::OnMontageCompleted);
				Task->ReadyForActivation();
			}
		}
	}
}

void UGA_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Jump::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
