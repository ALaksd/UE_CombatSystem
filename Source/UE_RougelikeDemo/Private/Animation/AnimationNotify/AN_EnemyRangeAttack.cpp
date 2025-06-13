// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimationNotify/AN_EnemyRangeAttack.h"
#include <Weapon/Projectile/RL_EnemyRangeAttack.h>
#include <Kismet/GameplayStatics.h>


void UAN_EnemyRangeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !AttackActorClass) return;

	if (!MeshComp || !AttackActorClass) return;

	AActor* OwnerActor = MeshComp->GetOwner();
	UWorld* World = MeshComp->GetWorld();

	if (!World || !OwnerActor) return;

	const FVector Center = MeshComp->GetSocketLocation(SocketName);
	

	if (RangeDamageParams.NumEffects <= 1)
	{
		FTransform SpawnTransform(OwnerActor->GetActorForwardVector().Rotation(), Center);
		RangeDamageParams.Ingisitor = OwnerActor;

		auto AttackActor = World->SpawnActorDeferred<ARL_EnemyRangeAttack>(
			AttackActorClass,
			SpawnTransform,
			OwnerActor,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		);

		if (AttackActor)
		{
			AttackActor->InitAttack(RangeDamageParams);
			UGameplayStatics::FinishSpawningActor(AttackActor, SpawnTransform);
		}
	}
	else
	{
		for (int32 i = 0; i < RangeDamageParams.NumEffects; ++i)
		{
			float AngleDeg = (360.f / RangeDamageParams.NumEffects) * i;
			float AngleRad = FMath::DegreesToRadians(AngleDeg);

			FVector Offset = FVector(FMath::Cos(AngleRad), FMath::Sin(AngleRad), 0.f) * RangeDamageParams.CircleRadius;
			FVector SpawnLoc = Center + Offset;
			FRotator SpawnRot = Offset.GetSafeNormal().Rotation();

			FTransform SpawnTransform(SpawnRot, SpawnLoc);
			RangeDamageParams.Ingisitor = OwnerActor;

			auto AttackActor = World->SpawnActorDeferred<ARL_EnemyRangeAttack>(
				AttackActorClass,
				SpawnTransform,
				OwnerActor,
				nullptr,
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
			);
			if (AttackActor)
			{
				AttackActor->InitAttack(RangeDamageParams);

				if (RangeDamageParams.bEnableCircularMove)
				{
					AttackActor->CenterPoint = Center; // 旋转中心为出生点
					AttackActor->CurrentAngleDeg = AngleDeg;
				}
				UGameplayStatics::FinishSpawningActor(AttackActor, SpawnTransform);
			}
		}
	}
}
