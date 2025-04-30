// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Structs.generated.h"

/**
 * 储存所有的Struct
 */

class UGameplayAbility;

USTRUCT(BlueprintType,Blueprintable)
struct FSkillList : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 技能名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillName;
	// 技能Icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon;
	
	// 技能属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillAttribute;
	// 技能描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
	// 技能GA
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayAbility>> GA;
	
};