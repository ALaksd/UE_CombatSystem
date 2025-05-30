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
#include "Engine/OverlapResult.h"
#include <Interface/RL_CombatInterface.h>
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>

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

	URL_AbilitySystemLibrary::GetLivePlayersInEllipse(
		OwnerActor,
		Hits,
		ActorsToIgnore,
		Center,
		RectangleParam,
		Rotation,
		true,   
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

void UANS_EnemyAttackDecision::CauseDamage(AActor* TargetActor, FVector HitLoction, FVector HitNormal)
{
	if (!DamageEffectClass || !OwnerActor) return;

	// 获取双方的ASC
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!SourceASC || !TargetASC) return;

	// 弹反判断条件 ---------------------------------------------------
	if (ParryDecision(TargetASC, SourceASC, HitLoction, HitNormal, TargetActor))
		return;

	// 无敌条件判断 ---------------------------------------------------
	const FGameplayTag InvincibleTag = FGameplayTag::RequestGameplayTag("State.Invincible");
	if (TargetASC->HasMatchingGameplayTag(InvincibleTag))
	{
		return;
	}

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

	//执行GameplayCue, 受击反馈
	
	FGameplayCueParameters CueParams;
	CueParams.Instigator = TargetActor; //击中者，就是玩家
	CueParams.Location = HitLoction; //击中位置
	CueParams.Normal = HitNormal;  //击中法向
	CueParams.NormalizedMagnitude = IntensityMultiplier;
	TargetASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Enemy.MeeleHit"), CueParams);

	URL_AbilitySystemLibrary::ApplyDamageByMagnitude(SourceASC, TargetASC, Context,DamageEffectClass, DamageTypeTag, Damage);

	// 减少理智值
	if (URL_SanitySubsystem* SanitySubsystem = UGameInstance::GetSubsystem<URL_SanitySubsystem>(TargetActor->GetWorld()->GetGameInstance()))
	{
		SanitySubsystem->ReduceSanity(Damage * ReduceSantiyFactor);
	}
}

bool UANS_EnemyAttackDecision::ParryDecision(UAbilitySystemComponent* TargetASC, UAbilitySystemComponent* SourceASC, FVector& HitLoction, FVector& HitNormal, AActor* TargetActor)
{
	bool bCanParry = false;

	const FGameplayTag ParryTag = FGameplayTag::RequestGameplayTag("State.BounceBack");
	const FGameplayTag ParryContinuousTag = FGameplayTag::RequestGameplayTag("State.BounceBack.Continuous");
	const FGameplayTag RedDamageTag = FGameplayTag::RequestGameplayTag("damage.Red");

	// 检查玩家是否有弹反Tag
	bool bPlayerHasParry = TargetASC->HasMatchingGameplayTag(ParryTag);
	bool bPlayerHasContinuous = TargetASC->HasMatchingGameplayTag(ParryContinuousTag);

	// 检查敌人是否有红光攻击Tag
	bool bEnemyRedAttack = SourceASC->HasMatchingGameplayTag(RedDamageTag);


	bCanParry = (bPlayerHasParry || bPlayerHasContinuous) && bEnemyRedAttack;

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
		UAS_Enemy* AS;
		if (OwnerActor->Implements<URL_EnemyInterface>())
		{
			AS = IRL_EnemyInterface::Execute_GetEnemyAttributeSet(OwnerActor);
			if (AS)
			{
				URL_AbilitySystemLibrary::ApplyChangeAttributeEffect(SourceASC, AS->GetStaminaAttribute(), -Breakingvalue);
			}
		}

		//弹反奖励
		//URL_AbilitySystemLibrary::ApplyTemporaryTag(TargetASC, FGameplayTag::RequestGameplayTag("State.BounceBack.Continuous"), 1.f);

		//3.敌人播放弹反受击动画（KonckDistance >= 200.f）,并且敌人后退
		if (KnockDistance >= 200.f)
		{
			UAnimInstance* AnimInstance = Cast<ACharacter>(OwnerActor)->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				UAnimMontage* ParryHitMontage = URL_AbilitySystemLibrary::GetEnemyConfig(OwnerActor)->ParryHitMontage;

				if (ParryHitMontage)
				{
					AnimInstance->StopAllMontages(0.1f);
					AnimInstance->Montage_Play(ParryHitMontage);
				}
				
			}

			FGameplayTag EnemyGuardBrokenTag = FGameplayTag::RequestGameplayTag("EnemyState.GuardBroken");
			SourceASC->AddLooseGameplayTag(EnemyGuardBrokenTag);

			FTimerHandle TimerHandle;
			OwnerActor->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [SourceASC,EnemyGuardBrokenTag]()
				{
					SourceASC->RemoveLooseGameplayTag(EnemyGuardBrokenTag);
				}
			, 1.0f, false);

		}
		//4.玩家后退(测试)
		if (TargetActor->Implements<URL_CombatInterface>())
		{
			IRL_CombatInterface::Execute_KnockBack(TargetActor, (OwnerActor->GetActorForwardVector()).GetSafeNormal() * KnockDistance);
		}

		// 弹反成功直接返回，不执行后续伤害逻辑
		return true;
	}
	return false;
}
