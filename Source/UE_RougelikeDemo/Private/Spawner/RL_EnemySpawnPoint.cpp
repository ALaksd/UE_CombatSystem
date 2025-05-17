// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/RL_EnemySpawnPoint.h"
#include "Character/Enemy_Base.h"
#include "Components/SplineComponent.h"

ARL_EnemySpawnPoint::ARL_EnemySpawnPoint()
{
	PatrolSpline = CreateDefaultSubobject<USplineComponent>("PatrolSpline");
	PatrolSpline->SetupAttachment(GetRootComponent());
}

void ARL_EnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	// 获取当前 Transform 并向 Z 轴偏移
	FTransform SpawnTransform = GetActorTransform();
	FVector NewLocation = SpawnTransform.GetLocation();
	NewLocation.Z += 300.f;
	SpawnTransform.SetLocation(NewLocation);

	// 延迟生成敌人
	CurrentSpawndEnemy = GetWorld()->SpawnActorDeferred<AEnemy_Base>(EnemyClass, SpawnTransform);
	if (!CurrentSpawndEnemy) return;

	// 传递样条组件
	CurrentSpawndEnemy->InitializePatrol(PatrolSpline);

	CurrentSpawndEnemy->FinishSpawning(SpawnTransform);
	CurrentSpawndEnemy->SpawnDefaultController();
}

void ARL_EnemySpawnPoint::DestroyEnemy()
{
	CurrentSpawndEnemy->Destroy();
	CurrentSpawndEnemy = nullptr;
}
