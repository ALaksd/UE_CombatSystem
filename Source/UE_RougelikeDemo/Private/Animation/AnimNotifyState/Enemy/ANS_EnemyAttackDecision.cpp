// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/Enemy//ANS_EnemyAttackDecision.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GAS/RL_AbilitySystemLibrary.h"
#include <AIController.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include "GAS/RL_AbilitySystemLibrary.h"
#include <AbilitySystemInterface.h>
#include "Engine/OverlapResult.h"
#include <Kismet/GameplayStatics.h>
#include <Interface/RL_EnemyInterface.h>

UANS_EnemyAttackDecision::UANS_EnemyAttackDecision()
{
	DamageTypeTag = FGameplayTag::RequestGameplayTag("damage.Normal");
}

void UANS_EnemyAttackDecision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 初始化攻击者
	OwnerActor = MeshComp->GetOwner();
	AlreadyHitActors.Empty();

	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	if (SourceASC)
	{
		SourceASC->AddLooseGameplayTag(DamageTypeTag);
		SourceASC->SetTagMapCount(DamageTypeTag, 1);
	}

	//攻击音效
	if (AttackSound && OwnerActor)
	{
		UGameplayStatics::PlaySoundAtLocation(OwnerActor, AttackSound, OwnerActor->GetActorLocation());
	}
	
}

void UANS_EnemyAttackDecision::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	SocketTrans = MeshComp->GetSocketTransform(AttackSocketName);
	FVector FinalCenter = SocketTrans.GetLocation() + SocketTrans.GetRotation().RotateVector(LocationOffset);
	FRotator FinalRotation = SocketTrans.GetRotation().Rotator();

	DetectAndApplyDamage(MeshComp, FinalCenter, FinalRotation);
}


void UANS_EnemyAttackDecision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// 清空已命中列表
	AlreadyHitActors.Empty();

	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	if (SourceASC)
	{
		SourceASC->RemoveLooseGameplayTag(DamageTypeTag);
		SourceASC->SetTagMapCount(DamageTypeTag, 0);
	}
}

void UANS_EnemyAttackDecision::DetectAndApplyDamage(USkeletalMeshComponent* MeshComp, FVector& Center, FRotator& Rotation)
{
	if (!OwnerActor) return;

	TArray<FHitResult> Hits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);

	URL_AbilitySystemLibrary::GetLivePlayersInArea(
		OwnerActor,
		Hits,
		ActorsToIgnore,
		Center,
		RectangleParam,
		SphereRadius,
		Rotation,
		DetectionShape,
		false,
		0.f,
		FColor::Emerald
	);


	// 3. 处理命中结果
	for (const FHitResult& Hit : Hits)
	{
		if (Hit.GetActor() && !AlreadyHitActors.Contains(Hit.GetActor()))
		{
			CauseDamage(Hit.GetActor(),Hit.ImpactPoint,Hit.ImpactNormal);
			AlreadyHitActors.Add(Hit.GetActor()); // 避免重复伤害
		}
	}
}

void UANS_EnemyAttackDecision::CauseDamage(AActor* TargetActor, FVector HitLocation, FVector HitNormal)
{
	if (!OwnerActor) return;

	// 准备伤害参数
	FDamageParams DamageParams;
	DamageParams.DamageEffectClass = DamageEffectClass;
	DamageParams.DamageTypeTag = DamageTypeTag;
	DamageParams.Damage = Damage;
	DamageParams.KnockDistance = KnockDistance;
	DamageParams.ReduceSanity = ReduceSantiy;
	DamageParams.BreakingValue = Breakingvalue;
	DamageParams.RestoreSanity = RestoreSanity;
	
	//敌人等级加成
	if (OwnerActor->Implements<URL_EnemyInterface>())
	{
		int32 EnemyLevel = IRL_EnemyInterface::Execute_GetEnemyLevel(OwnerActor);
		if (EnemyLevel > 1)
		{
			DamageParams.Damage *= EnemyLevel;
		}
		
	}

	// 使用工具函数处理伤害
	URL_AbilitySystemLibrary::ApplyEnemyDamage(
		OwnerActor,
		TargetActor,
		HitLocation,
		HitNormal,
		DamageParams
	);
}

