// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_EnemyRedAttack.h"
#include "NiagaraComponent.h"
#include <Interface/RL_EnemyInterface.h>

void UANS_EnemyRedAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp || !MeshComp->GetOwner()) return;

	// 获取当前 World 并检查是否为游戏世界
	UWorld* World = MeshComp->GetWorld();
	if (!World || !World->IsGameWorld()) return; // 仅在游戏世界执行

	if (MeshComp->GetOwner()->Implements<URL_EnemyInterface>())
	{
		UNiagaraComponent* NiagaraComp = IRL_EnemyInterface::Execute_GetRedAttackNiagaraComponent(MeshComp->GetOwner());
		if (NiagaraComp)
		{
			NiagaraComp->Activate();
		}
	}
}

void UANS_EnemyRedAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	
	if (!MeshComp) return;

	// 获取当前 World 并检查是否为游戏世界
	UWorld* World = MeshComp->GetWorld();
	if (!World || !World->IsGameWorld()) return; // 仅在游戏世界执行

	if (MeshComp->GetOwner()->Implements<URL_EnemyInterface>())
	{
		UNiagaraComponent* NiagaraComp = IRL_EnemyInterface::Execute_GetRedAttackNiagaraComponent(MeshComp->GetOwner());

		if (NiagaraComp && NiagaraComp->IsActive())
		{
			// 渐变停止而非立即销毁
			NiagaraComp->DeactivateImmediate();
		}
	}

	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
