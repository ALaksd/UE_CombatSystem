// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Base.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "GameFramework/Character.h"

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
	Super::PreAttributeChange(Attribute,NewValue);
}

void UAS_Base::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		//暂存伤害并设置IncomingDamage为0
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if (LocalIncomingDamage >= 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

			//是否致命伤害
			const bool bFatal = NewHealth <= 0.f;

			if (!bFatal)
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("Effect.HitReact")));
				// 获取来源角色
				FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
				UAbilitySystemComponent* SourceASC = EffectContextHandle.GetInstigatorAbilitySystemComponent();
				if (!SourceASC) return;
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
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
