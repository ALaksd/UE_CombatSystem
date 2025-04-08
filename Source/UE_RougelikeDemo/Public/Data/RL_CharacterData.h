// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RL_CharacterData.generated.h"

UENUM(BlueprintType)
enum class EEquipedWeapon : uint8
{
	None,             // Ĭ��
	NoWeapon,            // ͽ��
	OneHandWeapon,    // ��������(����������)
	TwoHandedWeapon,  // ˫������������������
	Spear,			  // ì
	Bow,              // ��
	Magic,            // ����
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walking,          // ����
	Jogging,          // ����
	Running,          // ����		    
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

	// ���캯������Ĭ��ֵ
	FMovementSetting()
		: MaxWalkSpeed(0.f)
		, MaxAcceleration(0.f)
		, BrakingDeceleration(0.f)
		, BrakingFrictionFactor(0.f)
		, BrakingFriction(0.f)
		, bUseSeparateBrakingFriction(false)
	{}
};