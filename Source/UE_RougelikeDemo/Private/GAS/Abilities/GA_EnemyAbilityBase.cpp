// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_EnemyAbilityBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include <Component/RL_EnemyMovementComponent.h>

void UGA_EnemyAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		//只播放动画

		URL_EnemyMovementComponent* EnemyMove = ActorInfo->AvatarActor->FindComponentByClass<URL_EnemyMovementComponent>();
		UAnimMontage* MontageToPlay = nullptr;

		if (EnemyMove)
		{
			TArray<FEnemySkills> EnemySkills = EnemyMove->GetEnemyConfig()->EnemySkills;
			for (const auto& EnemySkill : EnemySkills)
			{
				if (AbilityTags.HasTagExact(EnemySkill.AbilityTag))
				{
					MontageToPlay = EnemySkill.Animations[0].Montage;
					break;
				}
			}

		}

		if (MontageToPlay)
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
				Task->OnCompleted.AddDynamic(this, &UGA_EnemyAbilityBase::OnMontageCompleted);
				Task->OnCancelled.AddDynamic(this, &UGA_EnemyAbilityBase::OnMontageCompleted);
				Task->OnBlendOut.AddDynamic(this, &UGA_EnemyAbilityBase::OnMontageCompleted);
				Task->OnInterrupted.AddDynamic(this, &UGA_EnemyAbilityBase::OnMontageCompleted);
				Task->ReadyForActivation();
			}
		}

	}
}

void UGA_EnemyAbilityBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_EnemyAbilityBase::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);

}
