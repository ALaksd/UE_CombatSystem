// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Abilities/GameplayAbility.h>
#include "Data/RL_EnemyConfig.h"
#include "RL_EnemyMovementComponent.generated.h"


USTRUCT(BlueprintType)
struct FPatrolConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseSplinePatrol = true;	//是否巡逻

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WaitTimeAtPoint = 3.0f;	//每个巡逻点的等待时间

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLoopPath = true;	//是否循环巡逻
};

USTRUCT(BlueprintType)
struct FConfrontationConfig
{
	GENERATED_BODY()

	// 距离参数
	UPROPERTY(EditAnywhere)
	float MaxDistance = 1000.0f;

	UPROPERTY(EditAnywhere)
	float MidDistance = 600.f;

	UPROPERTY(EditAnywhere)
	float MinDistance = 300.0f;
};

UENUM(BlueprintType)
enum EMovementRange:uint8
{
	Max,
	Min,
	Mid,
};

// 技能配置结构体
USTRUCT(BlueprintType)
struct FSkillConfig : public FTableRowBase
{
	GENERATED_BODY()

	// 基础配置
	UPROPERTY(EditAnywhere, Category = "Skill")
	FGameplayTag SkillTag;

	// 概率权重
	UPROPERTY(EditAnywhere, Category = "Probability")
	float SelectionWeight = 1.0f;

	// 优先级（数值越大优先级越高）
	UPROPERTY(EditAnywhere, Category = "Priority")
	int32 PriorityLevel = 0;

	// 冷却时间（秒）
	UPROPERTY(EditAnywhere, Category = "Cooldown")
	float CooldownDuration = 5.0f;

	// 距离条件
	UPROPERTY(EditAnywhere, Category = "Distance")
	FFloatRange ValidDistance = FFloatRange(0.0f, 1000.0f);
};
class USplineComponent;
class UBehaviorTree;
/**
 * 敌人移动组件，用于敌人待机巡逻状态设置
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API URL_EnemyMovementComponent : public UActorComponent
{
	GENERATED_BODY()

	TArray<FVector> PatrolPoints;

public:	
	URL_EnemyMovementComponent();

	virtual void InitializeComponent() override;

	void InitializePatrolPoints(USplineComponent* NewSpline);

protected:
	virtual void BeginPlay() override;

	//样条线组件指针，从Owner获取
	UPROPERTY(BlueprintReadWrite, Category = "AI|Patrol")
	TObjectPtr<USplineComponent> PatrolSpline;

	// 巡逻参数
	UPROPERTY(EditAnywhere, Category = "AI|Patrol")
	FPatrolConfig PatrolConfig;

	//对峙参数
	UPROPERTY(EditAnywhere, Category = "AI|Confrontation")
	FConfrontationConfig ConfrontationConfig;

	//敌人配置表
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TObjectPtr<URL_EnemyConfig> EnemyConfig;

private:
	UPROPERTY()
	UStaticMeshComponent* WeaponMeshComponent;


public:			
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TArray<FVector>& GetPatrolPoints() { return PatrolPoints; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FPatrolConfig& GetPatrolConfig() { return PatrolConfig; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FConfrontationConfig& GetConfrontationConfig() { return ConfrontationConfig; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE URL_EnemyConfig* GetEnemyConfig() { return EnemyConfig; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform GetWeaopnSocketTransform();
};
