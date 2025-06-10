// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/Enemy/ANS_EnemyThroughAttack.h"
#include "Components/CapsuleComponent.h"

void UANS_EnemyThroughAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		// 获取胶囊体组件（通常是 ACharacter 中的 CapsuleComponent）
		UCapsuleComponent* CapsuleComponent = MeshComp->GetOwner()->FindComponentByClass<UCapsuleComponent>();

		if (CapsuleComponent)
		{
			CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // 仅忽略与角色的碰撞
		}
	}
}

void UANS_EnemyThroughAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp)
	{
		// 获取胶囊体组件（通常是 ACharacter 中的 CapsuleComponent）
		UCapsuleComponent* CapsuleComponent = MeshComp->GetOwner()->FindComponentByClass<UCapsuleComponent>();

		if (CapsuleComponent)
		{
			CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block); // 仅忽略与角色的碰撞
		}
	}
}
