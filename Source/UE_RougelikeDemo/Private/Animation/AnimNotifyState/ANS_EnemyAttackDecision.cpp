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
#include <AbilitySystemInterface.h>
#include <Interface/RL_EnemyInterface.h>
#include "GAS/AS/AS_Enemy.h"

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

	TArray<FHitResult> Hits;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(OwnerActor);

	URL_AbilitySystemLibrary::GetLivePlayersInEllipse(
		OwnerActor,
		Hits,
		ActorsToIgnore,
		Center,
		RectangleParam, // 前向500，横向300，垂直200
		Rotation,
		true,    // 开启调试绘制
		1.0f,    // 显示2秒
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

void UANS_EnemyAttackDecision::CauseDamage(AActor* TargetActor, FVector HitLoction, FVector HitNormal)
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

		// 1. 播放弹反Cue
		FGameplayCueParameters ParryCueParams;
		ParryCueParams.Instigator = OwnerActor; //击中者，敌人
		ParryCueParams.Location = HitLoction; //击中位置
		ParryCueParams.Normal = HitNormal;  //击中法向
		TargetASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Parry"), ParryCueParams);

		// 2. 减少属性
		// 动态创建GE实例（使用SourceASC作为Outer防止GC回收）
		UGameplayEffect* DynamicParryGE = NewObject<UGameplayEffect>(SourceASC, FName(TEXT("DynamicParryGE")));
		DynamicParryGE->DurationPolicy = EGameplayEffectDurationType::Instant; // 即时生效

		// 添加属性修饰符（这里减少体力）
		FGameplayModifierInfo& Modifier = DynamicParryGE->Modifiers.AddDefaulted_GetRef();
		UAS_Enemy* AS;
		if (OwnerActor->Implements<URL_EnemyInterface>())
		{
			AS = IRL_EnemyInterface::Execute_GetEnemyAttributeSet(OwnerActor);
			if (AS)
			{
				Modifier.Attribute = FGameplayAttribute(AS->GetStaminaAttribute()); 
				Modifier.ModifierOp = EGameplayModOp::Additive; // 修改类型：叠加
				FScalableFloat Magnitude;
				Magnitude.Value = -Damage * 2.0f; // 暂时造成敌人伤害两倍的属性削减
				Modifier.ModifierMagnitude = Magnitude;

				// 创建效果规格并应用
				FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
				SourceASC->ApplyGameplayEffectToSelf(DynamicParryGE,1.f, Context);
			}
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

	const float IntensityMultiplier = FMath::GetMappedRangeValueClamped(
		FVector2D(100.f, 300.f),
		FVector2D(1.0f, 2.0f),
		KnockDistance
	);

	FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, Context);

	//执行GameplayCue
	FGameplayCueParameters CueParams;
	CueParams.Instigator = TargetActor; //击中者，就是玩家
	CueParams.Location = HitLoction; //击中位置
	CueParams.Normal = HitNormal;  //击中法向
	CueParams.NormalizedMagnitude = IntensityMultiplier;
	TargetASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Enemy.MeeleHit"), CueParams);

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