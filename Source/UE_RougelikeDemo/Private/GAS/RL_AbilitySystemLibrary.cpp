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

void URL_AbilitySystemLibrary::GetLivePlayersInEllipse(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, const FVector& CenterLocation, const FVector EllipseRadii, FRotator Orientation, bool bDrawDebug, float DebugDuration, FColor DebugColor)
{
	OutOverlappingActors.Reset();

	// 参数有效性检查
	if (EllipseRadii.X <= 0 || EllipseRadii.Y <= 0 || EllipseRadii.Z <= 0) return;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);
	QueryParams.bTraceComplex = false;

	const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;

	// 创建椭圆碰撞形状（使用Box近似）
	FCollisionShape CollisionShape = FCollisionShape::MakeBox(EllipseRadii);
	const FQuat Rotation = Orientation.Quaternion();

	TArray<FOverlapResult> OverlapResults;
	bool bHasOverlaps = World->OverlapMultiByObjectType(
		OverlapResults,
		CenterLocation,
		Rotation,
		FCollisionObjectQueryParams(FCollisionObjectQueryParams::AllDynamicObjects),
		CollisionShape,
		QueryParams
	);

	// 调试绘制
	if (bDrawDebug)
	{
		DrawDebugBox(
			World,
			CenterLocation,
			EllipseRadii,
			Rotation,
			DebugColor,
			false,
			0.1f, // 每帧刷新
			0,
			2.0f  // 线条粗细
		);
	}

	// 处理检测结果
	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* HitActor = Result.GetActor();
		if (!HitActor) continue;

		if (HitActor->Implements<URL_CombatInterface>() &&
			!IRL_CombatInterface::Execute_isDead(HitActor))
		{
			if (AActor* Avatar = IRL_CombatInterface::Execute_GetAvatar(HitActor))
			{
				OutOverlappingActors.AddUnique(Avatar);
			}
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
