// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/RL_EnemyRangeAttack.h"
#include <NiagaraFunctionLibrary.h>
#include "Components/SphereComponent.h"
#include "GAS/RL_AbilitySystemLibrary.h"
#include <Interface/RL_PlayerInterface.h>
#include "AbilitySystemComponent.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "NiagaraComponent.h"

// Sets default values
ARL_EnemyRangeAttack::ARL_EnemyRangeAttack()
{
	PrimaryActorTick.bCanEverTick = true;

	NSComp = CreateDefaultSubobject<UNiagaraComponent>("NSComp");
	NSComp->SetupAttachment(GetRootComponent());
}

void ARL_EnemyRangeAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStart)
	{
		// 圆周运动
		if (bRotateAroundCenter)
		{
			CurrentAngleDeg += RotateSpeedDegPerSec * DeltaTime;
			float AngleRad = FMath::DegreesToRadians(CurrentAngleDeg);

			FVector Offset = FVector(FMath::Cos(AngleRad), FMath::Sin(AngleRad), 0.f) * (GetActorLocation() - CenterPoint).Size();
			SetActorLocation(CenterPoint + Offset);
		}

		// 向前移动
		if (bMoveForward)
		{
			AddActorWorldOffset(GetActorForwardVector() * MoveSpeed * DeltaTime, true);
		}
	}
	
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
	bPersistentDamageDetection = InRangeDamageParams.bPersistentDamageDetection;
	DetectionInterval = InRangeDamageParams.DetectionInterval;
	bMoveForward = InRangeDamageParams.bEnableForwardMove;
	bRotateAroundCenter = InRangeDamageParams.bEnableCircularMove;
	MoveSpeed = InRangeDamageParams.Speed;
	RotateSpeedDegPerSec = InRangeDamageParams.RotateSpeed;
	Ingisitor = InRangeDamageParams.Ingisitor;
}

void ARL_EnemyRangeAttack::StartAttack()
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Ingisitor);
	if (ASC)
	{
		ASC->AddLooseGameplayTag(DamageParams.DamageTypeTag, 1);
	}

	if (NSComp && NiagaraEffect)
	{
		NSComp->SetAsset(NiagaraEffect);
		NSComp->Activate(true);
	}

	bStart = true;

	if (bPersistentDamageDetection)
	{
		// 启动定时检测
		GetWorld()->GetTimerManager().SetTimer(
			DamageTimerHandle,
			this,
			&ARL_EnemyRangeAttack::PerformDamageDetection,
			DetectionInterval,
			true // 循环
		);
	}
	else
	{
		// 保留一次性检测作为回退方案
		PerformDamageDetection();
	}
}


void ARL_EnemyRangeAttack::PerformDamageDetection()
{
	TArray<FHitResult> Hits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Ingisitor);
	//AlreadyHitActors.Empty();

	URL_AbilitySystemLibrary::GetLivePlayersInArea(
		Ingisitor,
		Hits,
		ActorsToIgnore,
		GetActorLocation(),
		RectangleParams,
		SphereRadius,
		GetActorRotation(),
		DamageDetectionType,
		false,
		0.f,
		FColor::Orange
	);

	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && !AlreadyHitActors.Contains(HitActor))
		{
			URL_AbilitySystemLibrary::ApplyEnemyDamage(
				Ingisitor,
				HitActor,
				Hit.ImpactPoint,
				Hit.ImpactNormal,
				DamageParams
			);
			AlreadyHitActors.Add(HitActor);
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
	if (NSComp)
	{
		NSComp->Deactivate();   
	}

	GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
	Destroy();
}

