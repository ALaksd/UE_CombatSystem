// Fill out your copyright notice in the Description page of Project Settings.
#include "GAS/Abilities/GA_DefenseBase.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Component/CloseCombatComponent.h"
#include "Weapon/RL_Sword.h"

UGA_DefenseBase::UGA_DefenseBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	bRetriggerInstancedAbility = true;
}

void UGA_DefenseBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (IsActive())
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UCloseCombatComponent* CombatComp = ActorInfo->AvatarActor->FindComponentByClass<UCloseCombatComponent>();
	if (!CombatComp) { EndAbility(Handle, ActorInfo, ActivationInfo, true, true); return; }

	E_WeaponType WeaponType = CombatComp->GetCurrentWeapon()->WeaponType;

	UAnimMontage* MontageToPlay = nullptr;
	if (WeaponType == E_WeaponType::Sword)
		MontageToPlay = SwordMontage;
	else if (WeaponType == E_WeaponType::GreatSword)
		MontageToPlay = GreatSwordMontage;

	if (!MontageToPlay)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		MontageToPlay,
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


void UGA_DefenseBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_DefenseBase::OnMontageCompleted()
{
	GetActorInfo().AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.BounceBack"));
	GetActorInfo().AbilitySystemComponent->SetTagMapCount(FGameplayTag::RequestGameplayTag("State.BounceBack"), 0);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
