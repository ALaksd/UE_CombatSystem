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

	// 攻击者
	UPROPERTY()
	AActor* Ingisitor = nullptr;

	FVector Location;

	void InitAttack(FRangeDamageParams& InRangeDamageParams);
	void StartAttack();
	void EndAttack();

private:
	TArray<AActor*> AlreadyHitActors; // 已命中的目标
};
