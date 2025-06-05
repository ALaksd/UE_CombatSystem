// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimationNotify/AN_EnemyRangeAttack.h"
#include <Weapon/Projectile/RL_EnemyRangeAttack.h>
#include <Kismet/GameplayStatics.h>


void UAN_EnemyRangeAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !AttackActorClass) return;

	if (!MeshComp || !AttackActorClass) return;

	FVector SpawnLoc = MeshComp->GetSocketLocation(SocketName);
	FRotator SpawnRot = MeshComp->GetSocketRotation(SocketName);

	FTransform SpawnTransform(SpawnRot, SpawnLoc);

	AActor* OwnerActor = MeshComp->GetOwner();
	UWorld* World = MeshComp->GetWorld();

	if (World)
	{
		// 延迟生成（用于初始化前设置参数）
		auto AttackActor = World->SpawnActorDeferred<ARL_EnemyRangeAttack>(
			AttackActorClass,
			SpawnTransform,
			OwnerActor,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

		if (AttackActor)
		{
			// 你可以在这里设置变量、传递数据等
			AttackActor->InitAttack(SpawnLoc, NiagaraEffect,SphereRadius,DamageParams,OwnerActor);

			// 正式完成生成
			UGameplayStatics::FinishSpawningActor(AttackActor, SpawnTransform);
		}
	}
}
