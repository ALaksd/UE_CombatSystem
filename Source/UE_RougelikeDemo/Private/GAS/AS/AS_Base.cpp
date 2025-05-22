// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Base.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include <AbilitySystemBlueprintLibrary.h>

#include "Character/Enemy_Base.h"
#include "GameFramework/Character.h"
#include <GAS/RL_AbilitySystemLibrary.h>

UAS_Base::UAS_Base()
{

}

void UAS_Base::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DATETIMEPICKER(UAS_Base, Health);
	DOREPLIFETIME_CONDITION_NOTIFY(UAS_Base, Health, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAS_Base, MaxHealth, COND_None, REPNOTIFY_OnChanged);
}

void UAS_Base::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_Base, Health, OldHealth);
}

void UAS_Base::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAS_Base, MaxHealth, OldMaxHealth);
}

void UAS_Base::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	//限制血量不能超过最大血量
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxHealth());
	}
}


void UAS_Base::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	//Super::PreAttributeChange(Attribute,NewValue);
}

void UAS_Base::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		//暂存伤害并设置IncomingDamage为0
		float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if (LocalIncomingDamage >= 0.f)
		{
			if (AEnemy_Base* Enemy = Cast<AEnemy_Base>(GetOwningActor()))
				if (Enemy->bIsGuardBroken)
					LocalIncomingDamage = LocalIncomingDamage*1.2;
			
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			//是否致命伤害
			const bool bFatal = NewHealth <= 0.f;

			if (!bFatal)
			{
				//击退
				const FVector& KnockBackImpusle = URL_AbilitySystemLibrary::GetKonckBackImpulse(Props.EffectContextHandle);
				// 计算击退力的大小
				float KnockbackMagnitude = KnockBackImpusle.Size();  // 这个值越大，表示击退越强

				FGameplayEventData EventData;
				EventData.Instigator = Props.SourceAvatarActor;
				EventData.Target = Props.TargetAvatarActor;
				EventData.EventMagnitude = KnockbackMagnitude; // 传入击退力的大小
				EventData.ContextHandle = Props.EffectContextHandle;

				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
					Props.TargetAvatarActor,
					FGameplayTag::RequestGameplayTag(FName("Event.HitReact")),
					EventData
				);

			
				if (!KnockBackImpusle.IsNearlyZero(10.f))
				{
					if (Props.TargetAvatarActor->Implements<URL_CombatInterface>())
					{
						IRL_CombatInterface::Execute_KnockBack(Props.TargetAvatarActor, KnockBackImpusle);
					}
				}
			}

			else
			{
				if (Props.TargetAvatarActor->Implements<URL_CombatInterface>())
				{
					IRL_CombatInterface::Execute_Die(Props.TargetAvatarActor);
				}
					
			}

		}
	}
}

void UAS_Base::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	//以下是可以从Data中获得的信息
	//Source = causer of the effect, Target = target of the effect

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	//Target
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}
