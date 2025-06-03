// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemFragment.h"
#include "RLItemFragment_Skill.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLItemFragment_Skill : public URLInventoryItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName SkillName;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> GA;

	// 技能属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillAttribute;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FString Description;
	
};
