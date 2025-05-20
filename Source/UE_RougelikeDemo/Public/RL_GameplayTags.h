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
	FGameplayTag Attribute_Primary_DefensePower;


	/*次要属性*/
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxEndurance;
	FGameplayTag Attribute_Secondary_MaxAttachResource;

	/*Item标签*/
	//武器
	FGameplayTag Weapon_OneHand_Sword_1;
	FGameplayTag Weapon_OneHand_Sword_2;
	FGameplayTag Weapon_OneHand_Sword_3;
	FGameplayTag Weapon_TwoHand_Sword_1;
	FGameplayTag Weapon_TwoHand_Sword_2;
	FGameplayTag Weapon_TwoHand_Sword_3;
	//防具
	FGameplayTag Equip_Head_1;
	FGameplayTag Equip_Head_2;
	FGameplayTag Equip_Head_3;
	FGameplayTag Equip_Head_4;

	FGameplayTag Equip_Body_1;
	FGameplayTag Equip_Body_2;
	FGameplayTag Equip_Body_3;
	FGameplayTag Equip_Body_4;

	//道具u kl,
	FGameplayTag Use_1;
	FGameplayTag Use_2;
	FGameplayTag Use_3;
	FGameplayTag Use_4;
	FGameplayTag Use_5;
	FGameplayTag Use_6;

private:
	static FRLGameplayTags GameplayTags;
};

class UE_ROUGELIKEDEMO_API RL_GameplayTags
{
public:
	RL_GameplayTags();
	~RL_GameplayTags();
};
