// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/RL_Bow.h"
#include "RL_Staff.generated.h"

class ARL_Projectile_Fireball;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API ARL_Staff : public ARL_Bow
{
	GENERATED_BODY()

public:
	virtual void SpawnArrow(float Damage, FGameplayTag DamageTag, float Count, FVector CenterOffset,float Radius, float Angle) override;
	/// 圆弧参数
	// 中心偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc Settings")
	FVector ArcCenterOffset = FVector::ZeroVector;
	
	// 圆弧半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc Settings")
	float ArcRadius = 500.0f; 

	// 角度范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc Settings")
	float ArcAngleDegrees  = 500.0f;

	// 火球数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc Settings")
	int32 FireballCount = 0; 

private:
	// 圆弧中心
	FVector ArcCenter = FVector::ZeroVector;

	TArray<ARL_Projectile_Fireball*> SpawnFireballs(float Damage, FGameplayTag DamageTag,int NumProjectiles);
};

