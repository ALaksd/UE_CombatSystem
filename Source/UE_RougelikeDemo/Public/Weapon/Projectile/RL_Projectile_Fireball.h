// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RL_ProjectileBase.h"
#include "RL_Projectile_Fireball.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API ARL_Projectile_Fireball : public ARL_ProjectileBase
{
	GENERATED_BODY()

public:
	ARL_Projectile_Fireball();

	
	// 方向更新频率
	UPROPERTY(EditDefaultsOnly, Category = "Update")
	float TimeSinceLastTrackingUpdate;

	// 最大修正角度
	UPROPERTY(EditDefaultsOnly, Category = "Update")
	float MaxTrackingAngle;

	// 追踪停止距离
	UPROPERTY(EditDefaultsOnly, Category = "Update")
	float MaxTrackingDistance;

	// 要攻击的对象
	UPROPERTY()
	AActor* Target;
private:
	virtual void Tick(float DeltaSeconds) override;

	float TrackingStrength = 0.5;
	// 上一次更新的间隔
	float TimeSinceLastTracking = 0;
	// 更新速度方向
	void UpdateTracking(float DeltaSeconds);
	void ApplyDir(FVector Direction);

	// 是否已到达过关闭追踪距离,避免反复激活追踪
	bool bIsInDistance = false;
};
