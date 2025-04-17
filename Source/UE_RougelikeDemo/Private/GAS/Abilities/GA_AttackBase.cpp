// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Abilities/GA_AttackBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"

UGA_AttackBase::UGA_AttackBase()
{
	//����һ��Tag,��SendGameplayEventʱ�ɴ�������Tag������
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = TirggerTag;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);

	 InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_AttackBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//��ʱֻ���Ŷ���
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
			Task->ReadyForActivation();
		}
	}
}

void UGA_AttackBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Window.ComboWindow"))));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UGA_AttackBase::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
