// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GC/GCN_PlaySoundAndSystem.h"
#include "Sound/SoundBase.h"
#include "NiagaraFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

void UGCN_PlaySoundAndSystem::HandleGameplayCue(AActor* TargetActor, EGameplayCueEvent::Type EventType, const FGameplayCueParameters& Parameters)
{
	Super::HandleGameplayCue(TargetActor, EventType, Parameters);

	if (EventType == EGameplayCueEvent::Executed)
	{
		//播放效果的Actor
		AActor* SourceActor = Parameters.Instigator.Get();
		const FVector ImpactLocation = Parameters.Location;
		const FVector ImpactNormal = Parameters.Normal;

	   // 播放带强度控制的特效
		if (Effect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				Effect,
				TargetActor->GetActorLocation(),
				TargetActor->GetActorRotation()
			);
		}

		if (HitEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				this,
				HitEffect,
				ImpactLocation,
				UKismetMathLibrary::MakeRotFromZ(ImpactNormal)
			);
		}
	

		// 播放带强度控制的音效
		if (Sound)
		{

			UGameplayStatics::PlaySoundAtLocation(
				this,
				Sound,
				TargetActor->GetActorLocation(),
				FRotator::ZeroRotator
			);
		}

		// 增强屏幕震动效果（示例）
		if (APawn* OwnerPawn = Cast<APawn>(TargetActor))
		{
			if (APlayerController* PC = Cast<APlayerController>(OwnerPawn->GetController()))
			{
				// 根据强度调整震动强度
				PC->ClientStartCameraShake(
					HitCameraShake,
					2.f
				);
			}
		}

		SetPauseEffect(SourceActor);
	}
}
