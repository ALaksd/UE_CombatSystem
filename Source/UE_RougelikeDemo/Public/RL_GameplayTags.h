// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */

struct FRLGameplayTags
{
public:
	static const FRLGameplayTags Get() { return GameplayTags; }
	//初始化游戏标签
	static void InitializedNativeGameplayTags();

	/*主要属性*/
	FGameplayTag Attribute_Primary_Life;              
	FGameplayTag Attribute_Primary_Vigor;               
	FGameplayTag Attribute_Primary_Power;               


	/*次要属性*/
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxEndurance;
	FGameplayTag Attribute_Secondary_MaxAttachResource;

	/*Item标签*/
	//武器
	FGameplayTag Weapon_OneHand_Sword_1;
	FGameplayTag Weapon_OneHand_Sword_2;
	FGameplayTag Weapon_TwoHand_Sword_1;
	FGameplayTag Weapon_TwoHand_Sword_2;

	//防具
	FGameplayTag Equip_Head_1;

	//道具
	FGameplayTag Use_1;

private:
	static FRLGameplayTags GameplayTags;
};

class UE_ROUGELIKEDEMO_API RL_GameplayTags
{
public:
	RL_GameplayTags();
	~RL_GameplayTags();
};
