// Fill out your copyright notice in the Description page of Project Settings.


#include "RL_GameplayTags.h"
#include "GameplayTagsManager.h"

FRLGameplayTags FRLGameplayTags::GameplayTags;

RL_GameplayTags::RL_GameplayTags()
{
}

RL_GameplayTags::~RL_GameplayTags()
{
}

void FRLGameplayTags::InitializedNativeGameplayTags()
{
	/*主要属性*/
	GameplayTags.Attribute_Primary_Life = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Life"));
	GameplayTags.Attribute_Primary_Power = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Power"));
	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), FString("韧性"));

	/*次要属性*/
	GameplayTags.Attribute_Secondary_MaxAttachResource = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxAttachResource"));
	GameplayTags.Attribute_Secondary_MaxEndurance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxEndurance"));
	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"));

	/*Item标签*/
	GameplayTags.Weapon_OneHand_Sword_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.OneHand.Sword.1"));
	GameplayTags.Weapon_OneHand_Sword_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.OneHand.Sword.2"));
	GameplayTags.Weapon_TwoHand_Sword_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.TwoHand.Sword.1"));
	GameplayTags.Weapon_TwoHand_Sword_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Weapon.TwoHand.Sword.1"));

}
