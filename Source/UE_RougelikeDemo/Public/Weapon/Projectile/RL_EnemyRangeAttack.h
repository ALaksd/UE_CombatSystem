// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS/RL_AbilitySystemLibrary.h"
#include "RL_EnemyRangeAttack.generated.h"

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
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere)
	float LifeTime = 1.0f;

	float SphereRadius = 100.f;

	UPROPERTY()
	FDamageParams DamageParams;

	// 攻击者
	UPROPERTY()
	AActor* Ingisitor = nullptr;

	FVector Location;

	void InitAttack(FVector InLocation, UNiagaraSystem* InNiagaraEffect,float InSphereRadius,FDamageParams& InDamageParams,AActor* InIngisitor);
	void StartAttack();

private:
	TArray<AActor*> AlreadyHitActors; // 已命中的目标
};
