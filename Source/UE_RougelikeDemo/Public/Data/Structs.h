// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UE_RougelikeDemo/InventorySystem/Definition/RLItemDefinition_Skill.h"
#include "Structs.generated.h"

/**
 * 储存所有的Struct
 */

class UGameplayAbility;
class URLInventoryItemDefinition;

USTRUCT(BlueprintType,Blueprintable)
struct FRL_Skill : public FTableRowBase
{
	GENERATED_BODY()

public:
	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	// URLItemDefinition_Skill* Skill;
	
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
	TObjectPtr<URLInventoryItemDefinition> SkillDefinition;
	
};

USTRUCT(BlueprintType,Blueprintable)
struct FFirebalLocation
{
	GENERATED_BODY()

public:
	
	// 火球生成位置	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;

	// 火球生成旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;
};

USTRUCT(BlueprintType,Blueprintable)
struct FSkillIcon : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 武器Tag
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponTag;
	// 技能1Icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Skill_1;
	// 技能2Icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Skill_2;
	
	
};
