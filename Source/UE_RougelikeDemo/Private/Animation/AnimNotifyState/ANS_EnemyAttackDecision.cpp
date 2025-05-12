// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_EnemyAttackDecision.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GAS/RL_AbilitySystemLibrary.h"

void UANS_EnemyAttackDecision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 初始化攻击者
	OwnerActor = MeshComp->GetOwner();
	AlreadyHitActors.Empty();
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
}

void UANS_EnemyAttackDecision::DetectAndApplyDamage(USkeletalMeshComponent* MeshComp, FVector& Center, FRotator& Rotation)
{
	if (!OwnerActor) return;

	TArray<AActor*> HitActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);

	URL_AbilitySystemLibrary::GetLivePlayersInEllipse(
		OwnerActor,
		HitActors,
		ActorsToIgnore,
		Center,
		RectangleParam, // 前向500，横向300，垂直200
		Rotation,
		true,    // 开启调试绘制
		2.0f,    // 显示2秒
		FColor::Emerald
	);

	// 3. 处理命中结果
	for (AActor* HitActor : HitActors)
	{
		if (HitActor && !AlreadyHitActors.Contains(HitActor))
		{
			CauseDamage(HitActor);
			AlreadyHitActors.Add(HitActor); // 避免重复伤害
		}
	}
}

void UANS_EnemyAttackDecision::CauseDamage(AActor* TargetActor)
{
	if (!DamageEffectClass || !OwnerActor) return;

	// 1. 创建GE Spec
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	if (!SourceASC) return;

	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	Context.AddSourceObject(OwnerActor);

	FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, Context);

	// 2. 设置伤害值
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		DamageSpecHandle,
		DamageTypeTag,
		Damage
	);

	// 3. 应用伤害
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC)
	{
		SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
	}
}