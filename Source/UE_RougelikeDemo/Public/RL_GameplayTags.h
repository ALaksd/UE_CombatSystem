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
	//��ʼ����Ϸ��ǩ
	static void InitializedNativeGameplayTags();

	/*��Ҫ����*/
	FGameplayTag Attribute_Primary_Life;              
	FGameplayTag Attribute_Primary_Vigor;               
	FGameplayTag Attribute_Primary_Power;               


	/*��Ҫ����*/
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxEndurance;
	FGameplayTag Attribute_Secondary_MaxAttachResource;

private:
	static FRLGameplayTags GameplayTags;
};

class UE_ROUGELIKEDEMO_API RL_GameplayTags
{
public:
	RL_GameplayTags();
	~RL_GameplayTags();
};
