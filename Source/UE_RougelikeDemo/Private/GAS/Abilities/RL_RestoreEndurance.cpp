// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/RL_RestoreEndurance.h"
#include <GAS/AS/AS_Player.h>

void URL_RestoreEndurance::OnEnduranceChanged(const FOnAttributeChangeData& Data)
{
	// 检查是否是消耗（下降）
	if (Data.NewValue < LastEndurance)
	{
		// 重置计时器（推迟恢复）
		GetWorld()->GetTimerManager().ClearTimer(DelayHandle);
		GetWorld()->GetTimerManager().SetTimer(DelayHandle, this, &URL_RestoreEndurance::StartRestore, RecoverDelay, false);

		// 停止当前恢复（如果正在恢复）
		GetWorld()->GetTimerManager().ClearTimer(TickHandle);
	}

	LastEndurance = Data.NewValue;
}

void URL_RestoreEndurance::StartRestore()
{
	GetWorld()->GetTimerManager().SetTimer(TickHandle, this, &URL_RestoreEndurance::RestorerTick, TickInterval, true);
}

void URL_RestoreEndurance::RestorerTick()
{
	if (ASC && RestoreEnduranceEffectClass)
	{
		// 只在耐力未满时执行
		if (PlayerAS->GetEndurance() < PlayerAS->GetMaxEndurance())
		{
			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(RestoreEnduranceEffectClass, 1.f, ASC->MakeEffectContext());

			if (SpecHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}


void URL_RestoreEndurance::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ASC = GetAbilitySystemComponentFromActorInfo();
	PlayerAS = Cast<UAS_Player>(ASC->GetAttributeSet(UAS_Player::StaticClass()));


	// 获取初始值
	LastEndurance = PlayerAS->GetEndurance();

	// 绑定属性变化
	ASC->GetGameplayAttributeValueChangeDelegate(UAS_Player::GetEnduranceAttribute()).AddUObject(this, &URL_RestoreEndurance::OnEnduranceChanged);
}

void URL_RestoreEndurance::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
