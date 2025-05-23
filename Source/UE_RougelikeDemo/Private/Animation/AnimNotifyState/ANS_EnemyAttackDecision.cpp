// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_EnemyAttackDecision.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GAS/RL_AbilitySystemLibrary.h"
#include <System/RL_SanitySubsystem.h>
#include <AIController.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include "GAS/RL_AbilitySystemLibrary.h"

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

	// 获取双方的ASC
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!SourceASC || !TargetASC) return;

	// 弹反判断条件 ---------------------------------------------------
	bool bCanParry = false;

	const FGameplayTag ParryTag = FGameplayTag::RequestGameplayTag("Ability.BounceBack");
	const FGameplayTag RedDamageTag = FGameplayTag::RequestGameplayTag("damage.Red");

	// 检查玩家是否有弹反Tag
	bool bPlayerHasParry = TargetASC->HasMatchingGameplayTag(ParryTag);

	// 检查敌人是否有红光攻击Tag
	bool bEnemyRedAttack = SourceASC->HasMatchingGameplayTag(RedDamageTag);

	bCanParry = bPlayerHasParry && bEnemyRedAttack;

	if (bCanParry)
	{
		// 弹反成功处理 -----------------------------------------------
		// 获取敌人AI相关组件
		AAIController* AIController = Cast<AAIController>(OwnerActor->GetInstigatorController());
		UBehaviorTreeComponent* BTComponent = AIController ? AIController->FindComponentByClass<UBehaviorTreeComponent>() : nullptr;


		// 1. 停止行为树
		if (BTComponent)
		{
			// 中止当前运行的任务
			BTComponent->RestartTree();
		}

		// 2. 播放特殊受击动画
		if (USkeletalMeshComponent* Mesh = OwnerActor->FindComponentByClass<USkeletalMeshComponent>())
		{
			if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
			{
				// 立即停止所有动画
				AnimInstance->StopAllMontages(0.1f);

				UAnimMontage* ParryHitMontage = URL_AbilitySystemLibrary::GetEnemyConfig(OwnerActor)->ParryHitMontage;
				// 播放受击动画
				if (ParryHitMontage)
				{
					AnimInstance->Montage_Play(ParryHitMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				}
			}
		}

		// 3. 减少属性
		// 创建临时GE
		FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(ParryAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			SourceASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}

		// 弹反成功直接返回，不执行后续伤害逻辑
		return;
	}

	// 无敌条件判断 ---------------------------------------------------
	const FGameplayTag InvincibleTag = FGameplayTag::RequestGameplayTag("State.Invincible");
	if (TargetASC->HasMatchingGameplayTag(InvincibleTag))
	{
		return;
	}


	/**
	 * TODO:
	 * 受击反馈
	 * 音效
	 * 特效
	 */

	// 正常伤害处理 ---------------------------------------------------
	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	Context.AddSourceObject(OwnerActor);

	//传入击退参数
	FVector KonckBackVector = (OwnerActor->GetActorLocation() - TargetActor->GetActorLocation()).GetSafeNormal();
	URL_AbilitySystemLibrary::SetKonckBackImpulse(Context, KonckBackVector * KnockDistance);

	FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, Context);

	// 设置伤害值
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		DamageSpecHandle,
		DamageTypeTag,
		Damage
	);

	// 应用伤害
	SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);

	// 减少理智值
	if (URL_SanitySubsystem* SanitySubsystem = UGameInstance::GetSubsystem<URL_SanitySubsystem>(TargetActor->GetWorld()->GetGameInstance()))
	{
		SanitySubsystem->ReduceSanity(Damage * ReduceSantiyFactor);
	}
}