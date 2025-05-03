// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/RL_BaseWeapon.h"
#include "Sound/SoundCue.h"
#include "RL_Bow.generated.h"


UENUM(BlueprintType)
enum class EBowState : uint8
{
	Idle,
	Aim,
	Draw, 
};

UCLASS()
class UE_ROUGELIKEDEMO_API ARL_Bow : public ARL_BaseWeapon
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	USoundCue* BowDrawSound;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	EBowState BowState;

	//播放拉弓声音
	void PullBowStart();
	//停止播放拉弓声音
	void PullBowEnd();

	//最大拉弓时间
	float MaxPullTime = 1.0f;

	class UAudioComponent* SoundToPlay;
};
