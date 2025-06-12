// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/RL_EnemyRangeAttack.h"
#include <NiagaraFunctionLibrary.h>
#include "Components/SphereComponent.h"
#include "GAS/RL_AbilitySystemLibrary.h"
#include <Interface/RL_PlayerInterface.h>
#include "AbilitySystemComponent.h"
#include <AbilitySystemBlueprintLibrary.h>

// Sets default values
ARL_EnemyRangeAttack::ARL_EnemyRangeAttack()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARL_EnemyRangeAttack::BeginPlay()
{
	Super::BeginPlay();
	
	if (StartTime > 0.f)
	{
		FTimerHandle StartTimer;
		GetWorld()->GetTimerManager().SetTimer(StartTimer, this, &ARL_EnemyRangeAttack::StartAttack, StartTime, false);
	}
	else
	{
		StartAttack();
	}

	FTimerHandle EndTimer;
	GetWorld()->GetTimerManager().SetTimer(EndTimer, this, &ARL_EnemyRangeAttack::EndAttack, LifeTime, false);
}

void ARL_EnemyRangeAttack::InitAttack(FRangeDamageParams& InRangeDamageParams)
{
	NiagaraEffect = InRangeDamageParams.NiagaraEffect;
	LifeTime = InRangeDamageParams.LifeTime;
	StartTime = InRangeDamageParams.StartTime;
	NumEffects = InRangeDamageParams.NumEffects;
	CircleRadius = InRangeDamageParams.CircleRadius;
	DamageParams = InRangeDamageParams.DamageParams;
	SphereRadius = InRangeDamageParams.SphereRadius;
	RectangleParams = InRangeDamageParams.RectangleParams;
	DamageDetectionType = InRangeDamageParams.DamageDetectionType;
	Ingisitor = InRangeDamageParams.Ingisitor;
}

void ARL_EnemyRangeAttack::StartAttack()
{
	// 添加Tag
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Ingisitor);
	if (ASC)
	{
		ASC->AddLooseGameplayTag(DamageParams.DamageTypeTag, 1);
	}

	const FVector Center = GetActorLocation(); // 中心点

	if (NumEffects > 1)
	{
		for (int32 i = 0; i < NumEffects; ++i)
		{
			// 1️⃣ 计算角度
			float AngleDeg = (360.f / NumEffects) * i;
			float AngleRad = FMath::DegreesToRadians(AngleDeg);

			// 2️⃣ 计算位置
			FVector Offset = FVector(FMath::Cos(AngleRad), FMath::Sin(AngleRad), 0.f) * CircleRadius;
			FVector EffectLocation = Center + Offset;

			// 3️⃣ 可选：根据偏移方向旋转特效
			FVector Direction = Offset.GetSafeNormal();
			FRotator EffectRotation = Direction.Rotation();

			// 4️⃣ 生成特效
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				NiagaraEffect,
				EffectLocation,
				EffectRotation,
				FVector(1.f),
				true,
				true,
				ENCPoolMethod::None,
				true
			);
		}
	}
	else
	{
		// 4️⃣ 生成特效
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraEffect,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.f),
			true,
			true,
			ENCPoolMethod::None,
			true
		);
	}

	// 🟢 原有的伤害检测逻辑保持不变
	TArray<FHitResult> Hits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Ingisitor);

	URL_AbilitySystemLibrary::GetLivePlayersInArea(
		Ingisitor,
		Hits,
		ActorsToIgnore,
		GetActorLocation(),
		RectangleParams,
		SphereRadius,
		FRotator::ZeroRotator,
		DamageDetectionType,
		true,
		2.f,
		FColor::Emerald
	);

	for (const FHitResult& Hit : Hits)
	{
		if (Hit.GetActor() && !AlreadyHitActors.Contains(Hit.GetActor()))
		{
			URL_AbilitySystemLibrary::ApplyEnemyDamage(
				Ingisitor,
				Hit.GetActor(),
				Hit.ImpactPoint,
				Hit.ImpactNormal,
				DamageParams
			);
			AlreadyHitActors.Add(Hit.GetActor());
		}
	}
}


void ARL_EnemyRangeAttack::EndAttack()
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Ingisitor);
	if (ASC)
	{
		ASC->RemoveLooseGameplayTag(DamageParams.DamageTypeTag, 1);
	}

	Destroy();
}

