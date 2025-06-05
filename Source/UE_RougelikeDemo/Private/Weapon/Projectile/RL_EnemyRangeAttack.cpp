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
	
	StartAttack();

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ARL_EnemyRangeAttack::EndAttack, LifeTime, false);
}

void ARL_EnemyRangeAttack::InitAttack(FVector InLocation, UNiagaraSystem* InNiagaraEffect, float InSphereRadius, FDamageParams& InDamageParams, AActor* InIngisitor)
{
	Location = InLocation;
	NiagaraEffect = InNiagaraEffect;
	SphereRadius = InSphereRadius;
	DamageParams = InDamageParams;
	Ingisitor = InIngisitor;
}

void ARL_EnemyRangeAttack::StartAttack()
{
	//添加Tag
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Ingisitor);
	if(ASC)
	{
		ASC->AddLooseGameplayTag(DamageParams.DamageTypeTag, 1);
	}

	//生成特效
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		NiagaraEffect,
		Location,
		FRotator::ZeroRotator,
		FVector(1.f),
		true,   // Auto destroy
		true,   // Auto activate
		ENCPoolMethod::None,
		true    // PreCullCheck
	);
	
	//造成伤害
	TArray<FHitResult> Hits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Ingisitor);

	URL_AbilitySystemLibrary::GetLivePlayersInArea(
		Ingisitor,
		Hits,
		ActorsToIgnore,
		Location,
		FVector(20.f),
		SphereRadius,
		FRotator::ZeroRotator,
		EDetectionShapeType::Sphere,
		true,
		2.f,
		FColor::Emerald
	);


	// 3. 处理命中结果
	for (const FHitResult& Hit : Hits)
	{
		if (Hit.GetActor() && !AlreadyHitActors.Contains(Hit.GetActor()))
		{
			// 调用封装的伤害函数
			URL_AbilitySystemLibrary::ApplyEnemyDamage(
				Ingisitor,        // 敌人作为伤害来源
				Hit.GetActor(),        // 被攻击者
				Hit.ImpactPoint,
				Hit.ImpactNormal,
				DamageParams       // 参数可由动画通知或攻击体蓝图设定
			);
			AlreadyHitActors.Add(Hit.GetActor()); // 避免重复伤害
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

