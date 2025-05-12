// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/ASC_Base.h"

#include "GAS/Abilities/GA_Base.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "GAS/Abilities/GA_EnemyAbilityBase.h"

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

void UASC_Base::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return ;

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

	for(auto AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UASC_Base::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	//发送事件
	FGameplayEventData PayLoad;
	PayLoad.EventTag = AttributeTag;
	PayLoad.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, PayLoad);
}

void UASC_Base::EffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GamePlayEffectSpec,
                              FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer TagContainer;
	GamePlayEffectSpec.GetAllAssetTags(TagContainer);

	EffecctTags.Broadcast(TagContainer);
}
