// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/AS/AS_Player.h"
#include "RL_GameplayTags.h"

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
