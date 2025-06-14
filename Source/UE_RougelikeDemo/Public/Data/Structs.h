// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enums.h"
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
	FName SkillName = FName();
	// 技能Icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Icon = nullptr;
	
	// 技能属性
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillAttribute = FString();
	// 技能描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description = FString();
	// 技能GA
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<URLInventoryItemDefinition> SkillDefinition = nullptr;
	
};

USTRUCT(BlueprintType,Blueprintable)
struct FFirebalLocation
{
	GENERATED_BODY()

public:
	
	// 火球生成位置	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector();

	// 火球生成旋转
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator();
};

USTRUCT(BlueprintType,Blueprintable)
struct FSkillIcon : public FTableRowBase
{
	GENERATED_BODY()

public:
	// 武器Tag
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponTag = FGameplayTag();
	// 技能1Icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Skill_1 = nullptr;
	// 技能2Icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTexture2D> Skill_2 = nullptr;
};

USTRUCT(BlueprintType,Blueprintable)
struct F_CameraLocation : public FTableRowBase
{
	GENERATED_BODY()

public:
	//  位置类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_CameraLocation CameraLocation = E_CameraLocation::Normal;
	// 偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Offset = FVector();
	// 弹簧臂长度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BomLength = 0.f;
};
