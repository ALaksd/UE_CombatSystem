// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutomationBlueprintFunctionLibrary.h"
#include "Data/RL_EnemyConfig.h"
#include <Data/Enums.h>
#include "RL_AbilitySystemLibrary.generated.h"

class URL_OverlayWidgetController;
class URL_AttributeWidgetController;
class URL_InventoryWidgetController;
class URL_EquipWidgetController;
class UAbilitySystemComponent;
class AActor;
class UAnimInstance;
class URL_EnemyConfig;
class URL_SanitySubsystem;

USTRUCT(BlueprintType)
struct FDamageParams
{
	GENERATED_BODY()

	//伤害GE
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	//伤害类型Tag,damage.XXX
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag DamageTypeTag;

	//伤害值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	//击退距离
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockDistance;

	//减少理智值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReduceSanity;

	//弹反成功敌人减少的体力条
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BreakingValue;

	//弹反成功恢复理智值
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RestoreSanity;
};

USTRUCT(BlueprintType)
struct FRangeDamageParams
{
	GENERATED_BODY()

	//生成特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Effect")
	UNiagaraSystem* NiagaraEffect;

	//等待StartTime生成
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Effect")
	float StartTime = 0.f;
	// 生成数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	int32 NumEffects = 1;

	// 环形的半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float CircleRadius = 0.f;

	//球形伤害检测范围
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Damage")
	float SphereRadius = 100.f;

	//矩形伤害检测范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FVector RectangleParams = FVector(100.f);

	//检测类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EDetectionShapeType DamageDetectionType = EDetectionShapeType::Sphere;

	//伤害参数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	FDamageParams DamageParams;

	//伤害判定持续时间
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float LifeTime = 1.0f;

	UPROPERTY()
	FVector SpawnLocation;

	//伤害造成者
	UPROPERTY()
	AActor* Ingisitor;
};

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_AbilitySystemLibrary : public UAutomationBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static URL_OverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_AttributeWidgetController* GetAttributeMeauController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_InventoryWidgetController* GetInventoryWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_EquipWidgetController* GetEquipWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_LanternFlameController* GetLanternFlameWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void GetLivePlayerWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "Ability|Library", meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore"))
	static void GetLivePlayersInArea(const UObject* WorldContextObject, TArray<FHitResult>& OutHitResults, const TArray<AActor*>& ActorsToIgnore, const FVector& CenterLocation, const FVector BoxExtent, float SphereRadius, FRotator Orientation, EDetectionShapeType ShapeType, bool bDrawDebug, float DebugDuration, FColor DebugColor);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	static URL_EnemyConfig* GetEnemyConfig(AActor* Enemy);


	UFUNCTION(BlueprintPure, Category = "RPGAbilitySystemLibrary|GamepalyEffect")

	static FVector GetKonckBackImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "RPGAbilitySystemLibrary|GamepalyEffect")

	static FName GetHitBoneName(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "RPGAbilitySystemLibrary|GamepalyEffect")

	static FGameplayTag GetDamageTypeTag(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "RPGAbilitySystemLibrary|GamepalyEffect")
	static void SetKonckBackImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, FVector InKonckBackImpulse);

	UFUNCTION(BlueprintCallable, Category = "RPGAbilitySystemLibrary|GamepalyEffect")
	static void SetHitBoneName(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, FName InHitBoneName);

	UFUNCTION(BlueprintCallable, Category = "RPGAbilitySystemLibrary|GamepalyEffect")
	static void SetDamageTypeTag(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, FGameplayTag DamageTypeTag);

	//动态添加持续GE,增加Tag
	UFUNCTION(BlueprintCallable, Category = "RPGAbilitySystemLibrary|GamepalyEffect")
	static void ApplyTemporaryTag(UAbilitySystemComponent* ASC, const FGameplayTag& Tag, float Duration);

	//添加修改属性的GE
	UFUNCTION(BlueprintCallable, Category = "RPGAbilitySystemLibrary|GamepalyEffect")
	static FActiveGameplayEffectHandle ApplyChangeAttributeEffect(UAbilitySystemComponent* SourceASC, FGameplayAttribute bChangedAttribute, float InMagnitude, EGameplayEffectDurationType EffectDurationType = EGameplayEffectDurationType::Instant);

	//应用伤害（根据调用者设置）
	UFUNCTION(BlueprintCallable, Category = "RPGAbilitySystemLibrary|GamepalyEffect")
	static void ApplyDamageByMagnitude(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, FGameplayEffectContextHandle& Context,TSubclassOf<UGameplayEffect> DamageEffectClass, FGameplayTag DamageTag,float Damage);

	// 处理敌人造成的伤害
	UFUNCTION(BlueprintCallable, Category = "Combat")
	static void ApplyEnemyDamage(AActor* OwnerActor,AActor* TargetActor,const FVector& HitLocation,const FVector& HitNormal,const FDamageParams& DamageParams);

private:
	// 弹反判断逻辑
	static bool HandleParry(
		AActor* OwnerActor,
		AActor* TargetActor,
		const FVector& HitLocation,
		const FVector& HitNormal,
		const FDamageParams& DamageParams
	);
};
