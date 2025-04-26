// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_Other.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/AnimationNotify/AN_OtherAbility.h"
#include "Data/Enums.h"
#include "GameFramework/Character.h"

void UGA_Other::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//执行检查
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		//执行技能
		// 监听通知
		InitAnimation();
		if (UAnimMontage* MontageToPlay = Montage)
		{
			// 播放动画
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
				Task->OnCompleted.AddDynamic(this, &UGA_Other::OnMontageCompleted);
				Task->ReadyForActivation();
			}
		}
	}
	else
	{
		EndAbility(Handle,ActorInfo,ActivationInfo,true,false);
	}

	
}

void UGA_Other::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemBlueprintLibrary::RemoveLooseGameplayTags(GetAvatarActorFromActorInfo(), FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Window.ComboWindow"))));

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

void UGA_Other::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UGA_Other::InitAnimation()
{
	if (!Montage) return;

	//获取蒙太奇的所有通知Notify
	TArray<FAnimNotifyEvent> NotifyEvents = Montage->Notifies;
 
	for (FAnimNotifyEvent NotifyEvent : NotifyEvents)
	{
		UAN_OtherAbility* Notify = Cast<UAN_OtherAbility>(NotifyEvent.Notify);
		if (Notify)
		{
			//绑定通知
			Notify->OnSkill.AddUObject(this,&UGA_Other::OnReciveNotify);
			break;
		}
	}

}

void UGA_Other::OnReciveNotify(USkeletalMeshComponent* Mesh)
{
	if (ACharacter* Player = Cast<ACharacter>(Mesh->GetOwner()))
	{
		//向武器应用增幅
		if (UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Player))
		{
			//向武器应用增幅
			FGameplayEffectSpecHandle Handle = SourceASC->MakeOutgoingSpec(GameEffect,0,SourceASC->MakeEffectContext());
			SourceASC->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());

		}
	}
}
