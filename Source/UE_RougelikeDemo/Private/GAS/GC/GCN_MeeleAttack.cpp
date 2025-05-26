// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/GC/GCN_MeeleAttack.h"
#include "GameFramework/Character.h"
#include "GAS/RL_AbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundBase.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <Kismet/KismetMathLibrary.h>

void UGCN_MeeleAttack::HandleGameplayCue(AActor* TargetActor, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(TargetActor, EventType, Parameters);

	if (EventType == EGameplayCueEvent::Executed)
	{
		// 获取碰撞点和法线方向
		const FVector ImpactLocation = Parameters.Location;
		const FVector ImpactNormal = Parameters.Normal;
		const float NormalizedMagnitude = Parameters.NormalizedMagnitude;


		// 获取物理材质用于不同表面效果
		const UPhysicalMaterial* PhysMaterial = Parameters.PhysicalMaterial.Get();
		FGameplayTag SurfaceTag;

		//// 通过物理材质获取表面类型标签（需要自行实现材质标签映射）
		//if (PhysMaterial)
		//{
		//	SurfaceTag = URL_AbilitySystemLibrary::GetSurfaceTag(PhysMaterial);
		//}

	   // 播放带强度控制的特效
		if (ImpactEffect)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				ImpactEffect,
				ImpactLocation,
				UKismetMathLibrary::MakeRotFromZ(ImpactNormal) // 或 MakeRotFromX / MakeRotFromY
			);

			// 设置特效参数（需要Niagara系统中定义对应参数）
			if (NiagaraComp)
			{
				// 控制粒子数量
				NiagaraComp->SetIntParameter("SpawnCount", int32(30 * NormalizedMagnitude));
				// 控制粒子大小
				NiagaraComp->SetFloatParameter("ParticleScale", 50.f * NormalizedMagnitude);
				// 控制特效强度
				NiagaraComp->SetFloatParameter("SpawnVelocity", 200.f * NormalizedMagnitude);
			}
		}

		// 播放带强度控制的音效
		if (ImpactSound)
		{
			// 根据强度调整音量和音调
			const float Volume = FMath::Clamp(0.5f + NormalizedMagnitude * 0.5f, 0.5f, 1.5f);
			const float Pitch = FMath::Clamp(1.0f - NormalizedMagnitude * 0.2f, 0.8f, 1.2f);

			UGameplayStatics::PlaySoundAtLocation(
				this,
				ImpactSound,
				ImpactLocation,
				FRotator::ZeroRotator,
				Volume,     // 音量乘数
				Pitch,      // 音调乘数
				0.0f,
				nullptr,
				nullptr
			);
		}

		// 增强屏幕震动效果（示例）
		if (APawn* OwnerPawn = Cast<APawn>(Parameters.Instigator))
		{
			if (APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController()))
			{
				// 根据强度调整震动强度
				PC->ClientStartCameraShake(
					HitCameraShake,
					NormalizedMagnitude // 震动强度乘数
				);
			}
		}
	}
}

