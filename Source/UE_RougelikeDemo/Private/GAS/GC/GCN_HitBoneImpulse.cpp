// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GC/GCN_HitBoneImpulse.h"
#include "GameFramework/Character.h"
#include <GAS/RL_AbilitySystemLibrary.h>

void UGCN_HitBoneImpulse::HandleGameplayCue(AActor* TargetActor, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	if (!TargetActor || EventType != EGameplayCueEvent::Executed) return;

	ACharacter* Character = Cast<ACharacter>(TargetActor);
	if (!Character) return;

	USkeletalMeshComponent* Mesh = Character->GetMesh();
	if (!Mesh) return;

	// 从 Cue 参数中提取数据
	FName BoneName = URL_AbilitySystemLibrary::GetHitBoneName(Parameters.EffectContext);
	if (Parameters.RawMagnitude <= 0.f) return;

	FVector ImpulseDir = URL_AbilitySystemLibrary::GetKonckBackImpulse(Parameters.EffectContext).GetSafeNormal();
	float ForceScale = Parameters.RawMagnitude;

	// 开启骨骼物理
	Mesh->SetAllBodiesBelowSimulatePhysics(BoneName, true, true);
	Mesh->SetAllBodiesBelowPhysicsBlendWeight(BoneName, 0.5f);

	Mesh->SetEnableGravity(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	// 施加冲击力
	Mesh->AddImpulseToAllBodiesBelow(ImpulseDir * ForceScale, BoneName, true);

	// 定时关闭物理
	FTimerHandle TimerHandle;
	Character->GetWorldTimerManager().SetTimer(TimerHandle, [Mesh, BoneName]()
		{
			Mesh->SetAllBodiesBelowSimulatePhysics(BoneName, false, true);
		}, 0.3f, false);
}
