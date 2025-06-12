// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/Enemy/ANS_EnemyMeeleNiagara.h"

#include "NiagaraComponent.h"
#include <NiagaraFunctionLibrary.h>
#include "GAS/RL_AbilitySystemLibrary.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystemBlueprintLibrary.h>

void UANS_EnemyMeeleNiagara::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* OwnerActor = MeshComp->GetOwner();
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);

	URL_EnemyConfig* EnemyConfig = URL_AbilitySystemLibrary::GetEnemyConfig(OwnerActor);
	if (EnemyConfig)
	{
		UNiagaraSystem* NiagaraEffect = SourceASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("EnemyState.HalfLife")) ? EnemyConfig->LoadSpecialWeaponTrail() : EnemyConfig->LoadDefaultWeaponTrail();

		//创建并附加 Niagara 特效到插槽
		if (NiagaraEffect && MeshComp)
		{
			AttachedNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				NiagaraEffect,
				MeshComp,
				FXAttachSocketName,         // 插槽名，例如 "hand_r"
				FVector::ZeroVector,      // 位置偏移
				FRotator::ZeroRotator,    // 旋转偏移
				EAttachLocation::SnapToTarget,
				true,                     // bAutoDestroy
				true                      // bAutoActivate
			);
		}
	}
}

void UANS_EnemyMeeleNiagara::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AttachedNiagaraComp)
	{
		AttachedNiagaraComp->Deactivate();     // 优雅地停止
		AttachedNiagaraComp->DestroyComponent(); // 硬性清理（可选）
		AttachedNiagaraComp = nullptr;
	}
}
