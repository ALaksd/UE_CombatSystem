// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/RL_AbilitySystemLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "AbilitySystemComponent.h"
#include "Player/RL_PlayerState.h"
#include "UI/RL_HUD.h"
#include "UI/WidgetController/RL_AttributeWidgetController.h"
#include "UI/WidgetController/RL_LanternFlameController.h"
#include "UI/WidgetController/RL_OverlayWidgetController.h"
#include "Interface/RL_CombatInterface.h"
#include "Engine/OverlapResult.h"
#include <Component/RL_EnemyMovementComponent.h>
#include "GAS\RL_CustomGameplayEffectContext.h"
#include <AbilitySystemBlueprintLibrary.h>
#include <System/RL_SanitySubsystem.h>
#include <Interface/RL_EnemyInterface.h>
#include "GAS/AS/AS_Enemy.h"
#include <Interface/RL_CombatInterface.h>
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>

URL_OverlayWidgetController* URL_AbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARL_HUD* HUD = Cast<ARL_HUD>(PC->GetHUD()))
		{
			ARL_PlayerState* PS = PC->GetPlayerState<ARL_PlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			FWidgetControllerParams WidgetControllerParam(PC, PS, ASC, AS);
			return HUD->GetOverlayWidgetController(WidgetControllerParam);
		}
	}
	return nullptr;
}

URL_AttributeWidgetController* URL_AbilitySystemLibrary::GetAttributeMeauController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARL_HUD* HUD = Cast<ARL_HUD>(PC->GetHUD()))
		{
			ARL_PlayerState* PS = PC->GetPlayerState<ARL_PlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			FWidgetControllerParams WidgetControllerParam(PC, PS, ASC, AS);
			return HUD->GetAttributeWidgetController(WidgetControllerParam);
		}
	}
	return nullptr;
}

URL_InventoryWidgetController* URL_AbilitySystemLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARL_HUD* HUD = Cast<ARL_HUD>(PC->GetHUD()))
		{
			ARL_PlayerState* PS = PC->GetPlayerState<ARL_PlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			FWidgetControllerParams WidgetControllerParam(PC, PS, ASC, AS);
			return HUD->GetInventoryWidgetController(WidgetControllerParam);
		}
	}
	return nullptr;
}

URL_EquipWidgetController* URL_AbilitySystemLibrary::GetEquipWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARL_HUD* HUD = Cast<ARL_HUD>(PC->GetHUD()))
		{
			ARL_PlayerState* PS = PC->GetPlayerState<ARL_PlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			FWidgetControllerParams WidgetControllerParam(PC, PS, ASC, AS);
			return HUD->GetEquipWidgetController(WidgetControllerParam);
		}
	}
	return nullptr;
}

URL_LanternFlameController* URL_AbilitySystemLibrary::GetLanternFlameWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ARL_HUD* HUD = Cast<ARL_HUD>(PC->GetHUD()))
		{
			ARL_PlayerState* PS = PC->GetPlayerState<ARL_PlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			FWidgetControllerParams WidgetControllerParam(PC, PS, ASC, AS);
			return HUD->GetLanternFlameWidgetController(WidgetControllerParam);
		}
	}
	return nullptr;
}

void URL_AbilitySystemLibrary::GetLivePlayerWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	// 初始化查询参数
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);  // 忽略指定的Actors
	QueryParams.bTraceComplex = false;             // 简单碰撞检测

	// 用于存储重叠的Actors
	TArray<FOverlapResult> OverlapResults;

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		// 进行球形重叠查询
		bool bHasOverlaps = World->OverlapMultiByObjectType(
			OverlapResults,                            // 重叠结果
			SphereOrigin,                              // 球体原点
			FQuat::Identity,                           // 球体旋转
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),     // 查找Pawn对象类型
			FCollisionShape::MakeSphere(Radius),       // 球体形状和半径
			QueryParams                               // 查询参数
		);

		if (bHasOverlaps)
		{
			// 遍历重叠的结果并筛选玩家
			for (const FOverlapResult& Result : OverlapResults)
			{
				if (Result.GetActor())
				{
					if (Result.GetActor()->Implements<URL_CombatInterface>() && !IRL_CombatInterface::Execute_isDead(Result.GetActor()))
					{
						OutOverlappingActors.AddUnique(IRL_CombatInterface::Execute_GetAvatar(Result.GetActor()));
					}
				}
			}
		}
	}
}

void URL_AbilitySystemLibrary::GetLivePlayersInArea(const UObject* WorldContextObject,TArray<FHitResult>& OutHitResults,const TArray<AActor*>& ActorsToIgnore,const FVector& CenterLocation,const FVector BoxExtent,float SphereRadius,FRotator Orientation,EDetectionShapeType ShapeType,bool bDrawDebug,float DebugDuration,FColor DebugColor)
{
	OutHitResults.Reset();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);
	QueryParams.bTraceComplex = false;

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;

	FCollisionShape CollisionShape;
	if (ShapeType == EDetectionShapeType::Rectangle)
	{
		if (BoxExtent.IsNearlyZero()) return;
		CollisionShape = FCollisionShape::MakeBox(BoxExtent);
	}
	else if (ShapeType == EDetectionShapeType::Sphere)
	{
		if (SphereRadius <= 0.f) return;
		CollisionShape = FCollisionShape::MakeSphere(SphereRadius);
	}

	const FQuat RotationQuat = Orientation.Quaternion();
	TArray<FHitResult> HitResults;

	World->SweepMultiByObjectType(
		HitResults,
		CenterLocation,
		CenterLocation, // static overlap
		RotationQuat,
		FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllDynamicObjects),
		CollisionShape,
		QueryParams
	);

	if (bDrawDebug)
	{
		if (ShapeType == EDetectionShapeType::Rectangle)
		{
			DrawDebugBox(World, CenterLocation, BoxExtent, RotationQuat, DebugColor, false, DebugDuration, 0, 2.0f);
		}
		else
		{
			DrawDebugSphere(World, CenterLocation, SphereRadius, 16, DebugColor, false, DebugDuration, 0, 2.0f);
		}
	}

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) continue;

		if (HitActor->Implements<URL_CombatInterface>() &&
			!IRL_CombatInterface::Execute_isDead(HitActor))
		{
			OutHitResults.Add(Hit);
		}
	}
}


URL_EnemyConfig* URL_AbilitySystemLibrary::GetEnemyConfig(AActor* Enemy)
{
	if (!Enemy)
		return nullptr;
	URL_EnemyMovementComponent* EnemyMove = Enemy->FindComponentByClass<URL_EnemyMovementComponent>();
	if (EnemyMove)
	{
		return EnemyMove->GetEnemyConfig();
	}
	return nullptr;
}

FVector URL_AbilitySystemLibrary::GetKonckBackImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRLGameplayEffectContext* RPGEffectContext = static_cast<const FRLGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return RPGEffectContext->GetKnockBackImpulse();
	}
	return FVector();
}

FName URL_AbilitySystemLibrary::GetHitBoneName(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRLGameplayEffectContext* RPGEffectContext = static_cast<const FRLGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return RPGEffectContext->GetHitBoneName();
	}
	return FName();
}

FGameplayTag URL_AbilitySystemLibrary::GetDamageTypeTag(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FRLGameplayEffectContext* RPGEffectContext = static_cast<const FRLGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (RPGEffectContext->GetDamageType().IsValid())
		{
			return *RPGEffectContext->GetDamageType();
		}

	}
	return FGameplayTag();
}

void URL_AbilitySystemLibrary::SetKonckBackImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, FVector InKonckBackImpulse)
{
	if (FRLGameplayEffectContext* RPGEffectContext = static_cast<FRLGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RPGEffectContext->SetKnockBackImpulse(InKonckBackImpulse);
	}
}

void URL_AbilitySystemLibrary::SetHitBoneName(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, FName InHitBoneName)
{
	if (FRLGameplayEffectContext* RPGEffectContext = static_cast<FRLGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		RPGEffectContext->SetHitBoneName(InHitBoneName);
	}
}

void URL_AbilitySystemLibrary::SetDamageTypeTag(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag DamageTypeTag)
{
	if (FRLGameplayEffectContext* RPGEffectContext = static_cast<FRLGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(DamageTypeTag);
		RPGEffectContext->SetDamageType(DamageType);
	}
}

void URL_AbilitySystemLibrary::ApplyTemporaryTag(UAbilitySystemComponent* ASC, const FGameplayTag& Tag, float Duration)
{
	if (!ASC || !Tag.IsValid()) return;

	// 添加临时Tag
	ASC->AddLooseGameplayTag(Tag);

	// 创建定时器来移除
	FTimerHandle TimerHandle;
	ASC->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [ASC, Tag]()
		{
			ASC->RemoveLooseGameplayTag(Tag);
		}, Duration, false);
}

FActiveGameplayEffectHandle URL_AbilitySystemLibrary::ApplyChangeAttributeEffect(UAbilitySystemComponent* SourceASC, FGameplayAttribute bChangedAttribute, float InMagnitude, EGameplayEffectDurationType EffectDurationType)
{
	// 动态创建GE实例（使用SourceASC作为Outer防止GC回收）
	UGameplayEffect* DynamicParryGE = NewObject<UGameplayEffect>(SourceASC, FName(TEXT("DynamicParryGE")));
	DynamicParryGE->DurationPolicy = EffectDurationType;

	// 添加属性修饰符（这里减少体力）
	FGameplayModifierInfo& Modifier = DynamicParryGE->Modifiers.AddDefaulted_GetRef();

	Modifier.Attribute = bChangedAttribute;
	Modifier.ModifierOp = EGameplayModOp::Additive; 
	FScalableFloat Magnitude;
	Magnitude.Value = InMagnitude;
	Modifier.ModifierMagnitude = Magnitude;

	// 创建效果规格并应用
	FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceASC->ApplyGameplayEffectToSelf(DynamicParryGE, 1.f, Context);
	return ActiveGameplayEffectHandle;
}


void URL_AbilitySystemLibrary::ApplyDamageByMagnitude(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, FGameplayEffectContextHandle& Context,TSubclassOf<UGameplayEffect> DamageEffectClass, FGameplayTag DamageTag, float Damage)
{
	FGameplayEffectSpecHandle DamageSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, Context);

	// 设置伤害值
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
		DamageSpecHandle,
		DamageTag,
		Damage
	);

	// 应用伤害
	SourceASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
}

void URL_AbilitySystemLibrary::ApplyEnemyDamage(AActor* OwnerActor, AActor* TargetActor, const FVector& HitLocation, const FVector& HitNormal, const FDamageParams& DamageParams)
{
	if (!OwnerActor || !TargetActor || !DamageParams.DamageEffectClass) return;

	// 获取双方的ASC
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!SourceASC || !TargetASC) return;

	// 弹反判断条件 ---------------------------------------------------
	if (HandleParry(OwnerActor, TargetActor, HitLocation, HitNormal, DamageParams))
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

	// 传入击退参数
	FVector KnockBackVector = (OwnerActor->GetActorLocation() - TargetActor->GetActorLocation()).GetSafeNormal();
	URL_AbilitySystemLibrary::SetKonckBackImpulse(Context, KnockBackVector * DamageParams.KnockDistance);

	const float IntensityMultiplier = FMath::GetMappedRangeValueClamped(
		FVector2D(100.f, 300.f),
		FVector2D(1.0f, 2.0f),
		DamageParams.KnockDistance
	);

	// 执行GameplayCue, 受击反馈
	FGameplayCueParameters CueParams;
	CueParams.Instigator = TargetActor; // 击中者，就是玩家
	CueParams.Location = HitLocation; // 击中位置
	CueParams.Normal = HitNormal;  // 击中法向
	CueParams.NormalizedMagnitude = IntensityMultiplier;
	TargetASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Enemy.MeeleHit"), CueParams);

	URL_AbilitySystemLibrary::ApplyDamageByMagnitude(
		SourceASC,
		TargetASC,
		Context,
		DamageParams.DamageEffectClass,
		DamageParams.DamageTypeTag,
		DamageParams.Damage
	);

	// 减少理智值
	if (URL_SanitySubsystem* SanitySubsystem = UGameInstance::GetSubsystem<URL_SanitySubsystem>(TargetActor->GetWorld()->GetGameInstance()))
	{
		SanitySubsystem->ReduceSanity(DamageParams.ReduceSanity);
	}
}

bool URL_AbilitySystemLibrary::HandleParry(AActor* OwnerActor, AActor* TargetActor, const FVector& HitLocation, const FVector& HitNormal, const FDamageParams& DamageParams)
{
	// 获取双方的ASC
	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwnerActor);
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!SourceASC || !TargetASC) return false;

	const FGameplayTag ParryTag = FGameplayTag::RequestGameplayTag("State.BounceBack");
	const FGameplayTag ParryContinuousTag = FGameplayTag::RequestGameplayTag("State.BounceBack.Continuous");
	const FGameplayTag RedDamageTag = FGameplayTag::RequestGameplayTag("damage.Red");

	// 检查玩家是否有弹反Tag
	bool bPlayerHasParry = TargetASC->HasMatchingGameplayTag(ParryTag);
	bool bPlayerHasContinuous = TargetASC->HasMatchingGameplayTag(ParryContinuousTag);

	// 检查敌人是否有红光攻击Tag
	//bool bEnemyRedAttack = SourceASC->HasMatchingGameplayTag(RedDamageTag);
	bool bEnemyRedAttack = DamageParams.DamageTypeTag.MatchesTagExact(RedDamageTag);

	bool bCanParry = (bPlayerHasParry || bPlayerHasContinuous) && bEnemyRedAttack;

	if (bCanParry)
	{
		// 弹反成功处理 -----------------------------------------------
		// 1. 播放弹反Cue
		FGameplayCueParameters ParryCueParams;
		ParryCueParams.Instigator = OwnerActor; // 击中者，敌人
		ParryCueParams.Location = HitLocation; // 击中位置
		ParryCueParams.Normal = HitNormal;  // 击中法向
		TargetASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.Parry"), ParryCueParams);

		// 2. 减少属性
		UAS_Enemy* AS;
		if (OwnerActor->Implements<URL_EnemyInterface>())
		{
			AS = IRL_EnemyInterface::Execute_GetEnemyAttributeSet(OwnerActor);
			if (AS)
			{
				ApplyChangeAttributeEffect(
					SourceASC,
					AS->GetStaminaAttribute(),
					-DamageParams.BreakingValue
				);
			}
		}

		// 3. 敌人播放弹反受击动画
		if (DamageParams.KnockDistance >= GetEnemyConfig(OwnerActor)->HitThreshold)
		{
			if (ACharacter* EnemyCharacter = Cast<ACharacter>(OwnerActor))
			{
				UAnimInstance* AnimInstance = EnemyCharacter->GetMesh()->GetAnimInstance();
				UAnimMontage* ParryHitMontage = GetEnemyConfig(OwnerActor)->ParryHitMontage;
				if (AnimInstance && ParryHitMontage)
				{
					AnimInstance->StopAllMontages(0.1f);
					AnimInstance->Montage_Play(ParryHitMontage);
				}
			}

			FGameplayTag EnemyGuardBrokenTag = FGameplayTag::RequestGameplayTag("EnemyState.ParryHit");
			SourceASC->AddLooseGameplayTag(EnemyGuardBrokenTag);

			FTimerHandle TimerHandle;
			OwnerActor->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [SourceASC, EnemyGuardBrokenTag]()
				{
					SourceASC->RemoveLooseGameplayTag(EnemyGuardBrokenTag);
				}, 1.0f, false);
		}

		// 4. 玩家后退
		if (TargetActor->Implements<URL_CombatInterface>())
		{
			IRL_CombatInterface::Execute_KnockBack(
				TargetActor,
				(OwnerActor->GetActorForwardVector()).GetSafeNormal() * DamageParams.KnockDistance
			);
		}

		// 回复理智
		if (URL_SanitySubsystem* SanitySystem = OwnerActor->GetWorld()->GetGameInstance()->GetSubsystem<URL_SanitySubsystem>())
			SanitySystem->RestoreSanity(DamageParams.RestoreSanity);

		return true;
	}
	return false;
}
