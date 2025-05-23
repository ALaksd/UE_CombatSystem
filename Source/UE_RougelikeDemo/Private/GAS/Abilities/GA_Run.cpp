// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_Run.h"
#include "AbilitySystemComponent.h"
#include "Component/RL_MovementComponent.h"

UGA_Run::UGA_Run()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Run::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                              const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		ActorInfo->AvatarActor->FindComponentByClass<URL_MovementComponent>()->UpdateMovementState(EMovementState::Running);
		ActorInfo->AvatarActor->FindComponentByClass<UCharacterMovementComponent>()->bOrientRotationToMovement = true; // 是否朝向移动方向
		ActorInfo->AvatarActor->FindComponentByClass<UCharacterMovementComponent>()->bUseControllerDesiredRotation = false;

		UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
		FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
		FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GE_RunCost, 1.f, EffectContextHandle);
		ActiveRunConstHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
		
}

void UGA_Run::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	ASC->RemoveActiveGameplayEffect(ActiveRunConstHandle);
	ActorInfo->AvatarActor->FindComponentByClass<URL_MovementComponent>()->UpdateMovementState(EMovementState::Jogging);
	if (ActorInfo->AvatarActor->Tags.Contains(FName("IsLocking")))
	{
		ActorInfo->AvatarActor->FindComponentByClass<UCharacterMovementComponent>()->bOrientRotationToMovement = false; // 是否朝向移动方向
		ActorInfo->AvatarActor->FindComponentByClass<UCharacterMovementComponent>()->bUseControllerDesiredRotation = true;
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Run::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
