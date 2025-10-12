// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/GA_AttackBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"
#include <GAS/RL_AbilitySystemLibrary.h>
#include <GAS/AS/AS_Player.h>
#include <Player/RL_PlayerState.h>

#include "System/RL_SanitySubsystem.h"

UGA_AttackBase::UGA_AttackBase()
{
	//监听一个Tag,当SendGameplayEvent时可触发监听Tag的能力
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = TirggerTag;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_AttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// 扣除理智值
		if (bIsReduceSanity)
		{
			if (URL_SanitySubsystem* SanitySystem = GetWorld()->GetGameInstance()->GetSubsystem<URL_SanitySubsystem>())
			{
				if (SanitySystem->GetSanity() < ReduceValue)
				{
					EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
					return;
				}
				else
				{
					SanitySystem->ReduceSanity(ReduceValue);

				}
			}
		}

		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		if (UAnimMontage* MontageToPlay = AttackMontage)
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
				Task->OnCompleted.AddDynamic(this, &UGA_AttackBase::OnMontageCompleted);
				Task->OnCancelled.AddDynamic(this, &UGA_AttackBase::OnMontageCompleted);
				Task->OnBlendOut.AddDynamic(this, &UGA_AttackBase::OnMontageCompleted);
				Task->OnInterrupted.AddDynamic(this, &UGA_AttackBase::OnMontageCompleted);
				Task->ReadyForActivation();
			}
		}

		//增加霸体GE
		if (Dominance > 0.f)
		{
			ARL_PlayerState* PlayerState = Cast<ARL_PlayerState>(ActorInfo->OwnerActor);
			if (PlayerState)
			{
				AS = Cast<UAS_Player>(PlayerState->GetAttributeSet());
				if (AS)
				{
					AS->SetDominance(Dominance);
				}
			}
		}

	}
}

void UGA_AttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Window.ComboWindow"))));
	if (AS)
	{
		AS->SetDominance(0.f);
	}



	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UGA_AttackBase::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
