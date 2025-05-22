// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Prop/GA_HealthBottle.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Component/RL_MovementComponent.h"

UGA_HealthBottle::UGA_HealthBottle()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_HealthBottle::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		ActorInfo->AvatarActor->FindComponentByClass<URL_MovementComponent>()->UpdateMovementState(EMovementState::Walking);
		if (UseItemMontage)
		{
			UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				UseItemMontage,
				1.0f,
				NAME_None,
				false
			);

			if (PlayMontageTask)
			{
				PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_HealthBottle::OnMontageCompleted);
				PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_HealthBottle::OnMontageCompleted);
				PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_HealthBottle::OnMontageCompleted);
				PlayMontageTask->ReadyForActivation();
			}
		}
	}
}

void UGA_HealthBottle::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ActorInfo->AvatarActor->FindComponentByClass<URL_MovementComponent>()->UpdateMovementState(EMovementState::Jogging);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_HealthBottle::OnMontageCompleted()
{
	FGameplayEffectContextHandle Handle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(EffectClass, 1, Handle);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
