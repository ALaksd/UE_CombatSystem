// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Player.h"
#include "RL_GameplayTags.h"
#include "GameplayEffectExtension.h"
#include "Player/RL_PlayerState.h"
#include <Interface/RL_PlayerInterface.h>
#include "GameFramework/Character.h"

UAS_Player::UAS_Player()
{
	const FRLGameplayTags GameplayTags = FRLGameplayTags::Get();

	FAttributeSignature LifeDelegate;
	LifeDelegate.BindStatic(GetLifeAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Primary_Life, LifeDelegate);

	FAttributeSignature VigorDelegate;
	VigorDelegate.BindStatic(GetVigorAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Primary_Vigor, VigorDelegate);

	FAttributeSignature PowerDelegate;
	PowerDelegate.BindStatic(GetPowerAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Primary_Power, PowerDelegate);

	FAttributeSignature MaxHealthDelegate;
	MaxHealthDelegate.BindStatic(GetMaxHealthAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_MaxHealth, MaxHealthDelegate);

	FAttributeSignature MaxEnduranceDelegate;
	MaxEnduranceDelegate.BindStatic(GetMaxEnduranceAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_MaxEndurance, MaxEnduranceDelegate);

	FAttributeSignature MaxAttachResourceDelegate;
	MaxAttachResourceDelegate.BindStatic(GetMaxAttachResourceAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_MaxAttachResource, MaxAttachResourceDelegate);
}

void UAS_Player::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	//限制属性不能超过最大值
	if (Attribute == GetEnduranceAttribute())
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxEndurance());
	else if (Attribute == GetAttachResourceAttribute())
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxAttachResource());

	
}

void UAS_Player::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetMaxHealthAttribute() && bLevelHealth)
	{
		SetHealth(GetMaxHealth());
		bLevelHealth = false;
	}

	if (Attribute == GetMaxEnduranceAttribute() && bLevelEndurance)
	{
		SetEndurance(GetMaxEndurance());
		bLevelEndurance = false;
	}

	if (Attribute == GetMaxAttachResourceAttribute() && bLevelAttachResource)
	{
		SetAttachResource(GetMaxAttachResource());
		bLevelAttachResource = false;
	}
}

void UAS_Player::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	const FGameplayAttribute& ModifiedAttribute = Data.EvaluatedData.Attribute;

	// 检查是否是主属性之一
	if (!(ModifiedAttribute == GetLifeAttribute() ||
		ModifiedAttribute == GetVigorAttribute() ||
		ModifiedAttribute == GetPowerAttribute()))
	{
		return;
	}

	// 非正向变化（初始化、清零），跳过
	if (Data.EvaluatedData.Magnitude <= 0.f)
	{
		return;
	}


	// 获取来源角色
	FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* SourceASC = EffectContextHandle.GetInstigatorAbilitySystemComponent();
	if (!SourceASC) return;

	APlayerController* PlayerController = SourceASC->AbilityActorInfo->PlayerController.Get();
	APawn* Pawn = PlayerController ? PlayerController->GetPawn() : nullptr;
	ACharacter* SourceCharacter = Cast<ACharacter>(Pawn);

	if (SourceCharacter && SourceCharacter->Implements<URL_PlayerInterface>())
	{
		IRL_PlayerInterface::Execute_AddLevel(SourceCharacter);
		IRL_PlayerInterface::Execute_AddSpellPoints(SourceCharacter, 1);

		//加满Vital属性
		bLevelHealth = true;
		bLevelEndurance = true;
		bLevelAttachResource = true;
	}
}

