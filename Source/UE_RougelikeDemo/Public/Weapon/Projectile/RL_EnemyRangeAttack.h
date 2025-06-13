// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS/RL_AbilitySystemLibrary.h"
#include "RL_EnemyRangeAttack.generated.h"

/**
 * 敌人范围攻击
 */
class UNiagaraSystem;

UCLASS()
class UE_ROUGELIKEDEMO_API ARL_EnemyRangeAttack : public AActor
{
	GENERATED_BODY()

public:
	ARL_EnemyRangeAttack();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere,Category = "Effect")
	UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float LifeTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Effect")
	float StartTime = 0.f;

	// 生成数量
	UPROPERTY(EditAnywhere, Category = "Effect")
	int32 NumEffects = 3;      

	// 环形的半径
	UPROPERTY(EditAnywhere, Category = "Effect")
	float CircleRadius = 100.f; 

	UPROPERTY(EditAnywhere, Category = "Damage")
	FDamageParams DamageParams;

	//球形伤害检测范围
	UPROPERTY(EditAnywhere, Category = "Damage")
	float SphereRadius = 100.f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	FVector RectangleParams = FVector(100.f);

	UPROPERTY(EditAnywhere, Category = "Damage")
	EDetectionShapeType DamageDetectionType = EDetectionShapeType::Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bPersistentDamageDetection = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DetectionInterval = 0.1f; // 每隔多久检测一次

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bMoveForward = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bRotateAroundCenter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveSpeed = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotateSpeedDegPerSec = 90.f;

	// 攻击者
	UPROPERTY()
	AActor* Ingisitor = nullptr;

	UPROPERTY()
	FVector CenterPoint; // 圆周中心点

	UPROPERTY()
	float CurrentAngleDeg = 0.f; // 当前旋转角度

	FTimerHandle DamageTimerHandle;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* NSComp;

	void InitAttack(FRangeDamageParams& InRangeDamageParams);
	void StartAttack();
	void PerformDamageDetection();
	void EndAttack();

private:
	TArray<AActor*> AlreadyHitActors; // 已命中的目标

	bool bStart;
};
