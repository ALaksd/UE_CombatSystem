// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_ChargeAttack.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include <Abilities/Tasks/AbilityTask_PlayMontageAndWait.h>
#include "AbilitySystemComponent.h"
#include "System/RL_SanitySubsystem.h"
#include <Component/CloseCombatComponent.h>
#include "Weapon/RL_Sword.h"
#include <GAS/RL_AbilitySystemLibrary.h>

UGA_ChargeAttack::UGA_ChargeAttack()
{

}

void UGA_ChargeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	bReleased = false;
	CurrentChargeTime = 0.f;
	AvatarActor = ActorInfo->AvatarActor.Get();

	// 播放蓄力循环动画
	if (ChargeLoopMontage)
	{
		UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
		if (ASC)
		{
			// 创建并激活动画任务
			ChargeMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
				this,
				NAME_None,
				ChargeLoopMontage,
				1.0f,   // 播放速率
				NAME_None,
				true,   // 循环播放
				1.0f,   // 混合时间
				0,      // 开始section
				true    // 能力结束时停止
			);

			ChargeMontageTask->OnCompleted.AddDynamic(this, &UGA_ChargeAttack::OnMontageCompleted);
			ChargeMontageTask->OnInterrupted.AddDynamic(this, &UGA_ChargeAttack::OnMontageCompleted);
			ChargeMontageTask->OnCancelled.AddDynamic(this, &UGA_ChargeAttack::OnMontageCompleted);
			ChargeMontageTask->ReadyForActivation();
		}
	}


	// 开始蓄力 Tick
	GetWorld()->GetTimerManager().SetTimer(ChargeTimerHandle, this, &UGA_ChargeAttack::TickCharging, 0.1f, true);

	// 等待输入释放
	InputReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	InputReleaseTask->OnRelease.AddDynamic(this, &UGA_ChargeAttack::OnReleased);
	InputReleaseTask->ReadyForActivation();
}

void UGA_ChargeAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility,bWasCancelled);
}

void UGA_ChargeAttack::TickCharging()
{
	CurrentChargeTime = FMath::Min(CurrentChargeTime + 0.1f, MaxChargeTime);

	//持续减少理智,蓄力时间少于0.2秒不扣除理智
	URL_SanitySubsystem* SanitySystem = GetWorld()->GetGameInstance()->GetSubsystem<URL_SanitySubsystem>();
	if (SanitySystem && CurrentChargeTime < MaxChargeTime && CurrentChargeTime > 0.2f)
	{
		SanitySystem->ReduceSanity(PerTimeSanity);
		if (SanitySystem->GetSanity() > 0)
		{
			float PowerRatio = CurrentChargeTime / MaxChargeTime;
			ScaledPower = FMath::Lerp(BasePower, MaxPower, PowerRatio);
		}
		else 
		{
			OnReleased(CurrentChargeTime);
		}
	}

}

void UGA_ChargeAttack::OnReleased(float TimeHeld)
{
	if (bReleased) return; // 防止重复调用
	bReleased = true;

	// 停止蓄力动画
	StopChargingAnimation();

	if (InputReleaseTask && InputReleaseTask->IsActive())
	{
		InputReleaseTask->EndTask();
	}

	//找到武器并应用GE
	UCloseCombatComponent* CloseCombatComp = AvatarActor->FindComponentByClass<UCloseCombatComponent>();
	if (CloseCombatComp)
	{
		ARL_Sword* CurrentWeapon = CloseCombatComp->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			WeaponASC = CurrentWeapon->GetWeaponASC();
			UAS_Weapon* WeaponAS = CurrentWeapon->GetWeaponAS();
			if (WeaponASC && WeaponAS)
			{
				float CurrentDamage = WeaponAS->GetDamage() * ScaledPower;
				DamageActiveEffectHandle = URL_AbilitySystemLibrary::ApplyChangeAttributeEffect(WeaponASC, WeaponAS->GetDamageAttribute(), CurrentDamage, EGameplayEffectDurationType::Infinite);
			}
		}
	}

	// 播放释放攻击动画
	if (ChargeReleaseMontage && TimeHeld <= 5.3f) //5.3f是蓄力动画的时长
	{
		UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			NAME_None,
			ChargeReleaseMontage,
			1.f,
			NAME_None,
			false
		);
		MontageTask->OnCompleted.AddDynamic(this, &UGA_ChargeAttack::OnMontageCompleted);
		MontageTask->OnInterrupted.AddDynamic(this, &UGA_ChargeAttack::OnMontageCompleted);
		MontageTask->OnCancelled.AddDynamic(this, &UGA_ChargeAttack::OnMontageCompleted);
		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
	}
}

void UGA_ChargeAttack::StopChargingAnimation()
{
	// 清除蓄力计时器
	GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
	// 停止蓄力循环动画

	if (ChargeMontageTask && ChargeMontageTask->IsActive())
	{
		ChargeMontageTask->EndTask();
	}
}


void UGA_ChargeAttack::OnMontageCompleted()
{
	//清除增加伤害的GE
	if (DamageActiveEffectHandle.IsValid())
	{
		WeaponASC->RemoveActiveGameplayEffect(DamageActiveEffectHandle);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
