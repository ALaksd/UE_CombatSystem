// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_EnemyAbilityBase.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include <Interface/RL_CombatInterface.h>
#include <Interface/RL_EnemyInterface.h>

void UGA_EnemyAbilityBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		AActor* SourceActor = GetAvatarActorFromActorInfo();
		// 启动定时器，每0.1秒更新一次朝向
		GetWorld()->GetTimerManager().SetTimer(
			FacingUpdateTimerHandle,
			this,
			&UGA_EnemyAbilityBase::UpdateFacingDirection,
			0.1f, // 间隔时间
			true  // 循环执行
		);

		//播放动画
		if (UAnimMontage* MontageToPlay = AbilityMontage)
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
	GetWorld()->GetTimerManager().ClearTimer(FacingUpdateTimerHandle);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UGA_EnemyAbilityBase::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	
}


void UGA_EnemyAbilityBase::UpdateFacingDirection()
{
	AActor* SourceActor = GetAvatarActorFromActorInfo();
	//获取攻击目标,并将朝向转向攻击目标
	if (SourceActor->Implements<URL_CombatInterface>())
	{
		if (SourceActor->Implements<URL_EnemyInterface>())
		{
			FVector TargerLocation = IRL_EnemyInterface::Execute_GetCombatTarget(SourceActor)->GetActorLocation();
			IRL_CombatInterface::Execute_SetFacingTarget(SourceActor, TargerLocation);
		}
	}
}
