// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RL_CharacterData.generated.h"

UENUM(BlueprintType)
enum class EEquipedWeapon : uint8
{
	None,             // 默认
	NoWeapon,            // 徒手
	OneHandWeapon,    // 单手武器(剑，刀，盾)
	TwoHandedWeapon,  // 双手武器（重型武器）
	Spear,			  // 矛
	Bow,              // 弓
	Magic,            // 法术
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walking,          // 行走
	Jogging,          // 慢跑
	Running,          // 疾跑		    
};

USTRUCT(BlueprintType)
struct FMovementSetting
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Movement")
	float MaxWalkSpeed;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Movement")
	float MaxAcceleration;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Movement")
	float BrakingDeceleration;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Movement")
	float BrakingFrictionFactor;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Movement")
	float BrakingFriction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Movement")
	bool bUseSeparateBrakingFriction;

	// 构造函数设置默认值
	FMovementSetting()
		: MaxWalkSpeed(0.f)
		, MaxAcceleration(0.f)
		, BrakingDeceleration(0.f)
		, BrakingFrictionFactor(0.f)
		, BrakingFriction(0.f)
		, bUseSeparateBrakingFriction(false)
	{}
};