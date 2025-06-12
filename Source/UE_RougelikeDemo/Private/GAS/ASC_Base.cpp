// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ASC_Base.h"

#include "GAS/Abilities/GA_Base.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "GAS/Abilities/GA_EnemyAbilityBase.h"
#include <GameplayTagsManager.h>

void UASC_Base::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UASC_Base::EffectApplied);
}

void UASC_Base::AddCharacterAbility(const TSubclassOf<UGameplayAbility> StartupAbilities)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(StartupAbilities,1);
	if (const UGA_Base* Ability = Cast<UGA_Base>(AbilitySpec.Ability))
	{
		AbilitySpec.DynamicAbilityTags.AddTag(Ability->StartInputTag);
		GiveAbility(AbilitySpec);
	}
}

void UASC_Base::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		AddCharacterAbility(AbilityClass);
	}
}

void UASC_Base::AddEnemyAbility(const FEnemySkills& SkillInfo)
{
	// 生成 AbilitySpec（默认等级为 1）
	FGameplayAbilitySpec AbilitySpec(SkillInfo.AbilityClass, 1);

	// 提取标签
	UGA_EnemyAbilityBase* AbilityCDO = Cast<UGA_EnemyAbilityBase>(SkillInfo.AbilityClass->GetDefaultObject());
	if (AbilityCDO)
	{
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityCDO->StartInputTag);
	}

	// 注册能力
	GiveAbility(AbilitySpec);

}


void UASC_Base::AddEnemyAbilities(const TArray<FEnemySkills>& AllSkills)
{
	for (const FEnemySkills& Skill : AllSkills)
	{
		if (Skill.AbilityClass)
		{
			AddEnemyAbility(Skill);
		}
	}
}


void UASC_Base::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (auto AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UASC_Base::AbilityPressTagInput(const FGameplayTag& InputTag)
{
	// 判断输入的标签是否有效
	if (!InputTag.IsValid())   return;

	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())  // 遍历所有可以激活的能力
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))  // 检查该能力是否与输入标签匹配
		{
			AbilitySpecInputPressed(AbilitySpec);  // 表示按下了与该能力相关的输入
			if (!AbilitySpec.IsActive())  // 如果该能力当前未激活
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}
}

void UASC_Base::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return ;

	FScopedAbilityListLock ActiveScopeLoc(*this);
	//检查是否有启动键与当前输入键相同的Ability，并且此能力现在没有启用则启动它
	for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (!AbilitySpec.IsActive())
			{
				//执行Ability
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UASC_Base::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return ;

	FScopedAbilityListLock ActiveScopeLoc(*this);

	for(auto AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UASC_Base::UpgradeAttribute()
{
	//这里升一级增加所有主属性1点
	FGameplayTagContainer AttributeTags = UGameplayTagsManager::Get().RequestGameplayTagChildren(FGameplayTag::RequestGameplayTag("Attributes.Primary"));
	for (FGameplayTag Tag : AttributeTags)
	{
		//发送事件
		FGameplayEventData PayLoad;
		PayLoad.EventTag = Tag;
		PayLoad.EventMagnitude = 1.f;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), Tag, PayLoad);
	}

}

void UASC_Base::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GamePlayEffectSpec,
                              FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer TagContainer;
	GamePlayEffectSpec.GetAllAssetTags(TagContainer);

	EffecctTags.Broadcast(TagContainer);
}
