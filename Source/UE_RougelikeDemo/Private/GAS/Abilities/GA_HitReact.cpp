// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_HitReact.h"
#include <Interface/RL_CombatInterface.h>
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"
#include <Interface/RL_EnemyInterface.h>
#include <Interface/RL_PlayerInterface.h>
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <GAS/RL_AbilitySystemLibrary.h>
#include "AbilitySystemComponent.h"

UGA_HitReact::UGA_HitReact()
{
	//监听一个Tag,当SendGameplayEvent时可触发监听Tag的能力
	FAbilityTriggerData TriggerData;
	TriggerData.TriggerTag = FGameplayTag::RequestGameplayTag("Event.HitReact") ;
	TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;

	AbilityTriggers.Add(TriggerData);

	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_HitReact::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanbeHit())
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

		// 获取击退大小和方向
		float KnockbackMagnitude = TriggerEventData ? TriggerEventData->EventMagnitude : 0.f;
		//FVector KnockbackDirection = (TriggerEventData->Instigator->GetActorLocation() - ActorInfo->AvatarActor->GetActorLocation()).GetSafeNormal();  // 方向示例

		const FVector KnockbackDirection = URL_AbilitySystemLibrary::GetKonckBackImpulse(TriggerEventData->ContextHandle).GetSafeNormal();

		//获取击中骨骼名
		const FName HitBoneName = URL_AbilitySystemLibrary::GetHitBoneName(TriggerEventData->ContextHandle);

		// 根据大小和方向选择适当的受击动画
		UAnimMontage* HitMontage = nullptr;

		// 小击退，轻微受击：基于方向划分四个方向
		if (KnockbackMagnitude < 200.f)
		{
			FVector Forward = ActorInfo->AvatarActor->GetActorForwardVector();
			FVector Right = ActorInfo->AvatarActor->GetActorRightVector();
			FVector Backward = -Forward;
			FVector Left = -Right;

			float DotForward = FVector::DotProduct(KnockbackDirection, Forward);
			float DotBackward = FVector::DotProduct(KnockbackDirection, Backward);
			float DotLeft = FVector::DotProduct(KnockbackDirection, Left);
			float DotRight = FVector::DotProduct(KnockbackDirection, Right);

			if (DotForward > 0.7f) // 正前方
			{
				HitMontage = IRL_CombatInterface::Execute_GetLightHitReactFrontMontage(ActorInfo->AvatarActor.Get());
				UE_LOG(LogTemp, Warning, TEXT("正前方"));
			}
			else if (DotBackward > 0.7f) // 正后方
			{
				HitMontage = IRL_CombatInterface::Execute_GetLightHitReactBackMontage(ActorInfo->AvatarActor.Get());
				UE_LOG(LogTemp, Warning, TEXT("正后方"));
			}
			else if (DotLeft > 0.7f) // 左侧
			{
				HitMontage = IRL_CombatInterface::Execute_GetLightHitReactLeftMontage(ActorInfo->AvatarActor.Get());
				UE_LOG(LogTemp, Warning, TEXT("左侧"));
			}
			else if (DotRight > 0.7f) // 右侧
			{
				HitMontage = IRL_CombatInterface::Execute_GetLightHitReactRightMontage(ActorInfo->AvatarActor.Get());
				UE_LOG(LogTemp, Warning, TEXT("右侧"));
			}
		}
		// 大击退：强力受击
		else
		{
			HitMontage = IRL_CombatInterface::Execute_GetHeavyHitReactMontage(ActorInfo->AvatarActor.Get());
		}

		// 播放动画
		if (HitMontage)
		{
			UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				HitMontage,
				1.0f,
				NAME_None,
				false
			);

			if (Task)
			{
				Task->OnCompleted.AddDynamic(this, &UGA_HitReact::OnMontageCompleted);
				Task->OnCancelled.AddDynamic(this, &UGA_HitReact::OnMontageCompleted);
				Task->OnBlendOut.AddDynamic(this, &UGA_HitReact::OnMontageCompleted);
				Task->OnInterrupted.AddDynamic(this, &UGA_HitReact::OnMontageCompleted);
				Task->ReadyForActivation();
			}
		}
	}
	
}


void UGA_HitReact::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGA_HitReact::CanbeHit()
{
	return true;
}

void UGA_HitReact::OnMontageCompleted()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(ActiveHitReact);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

