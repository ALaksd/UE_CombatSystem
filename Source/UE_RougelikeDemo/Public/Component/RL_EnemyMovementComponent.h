// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Abilities/GameplayAbility.h>
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

	// 移动参数
	UPROPERTY(EditAnywhere)
	float ChaseSpeed = 600.0f;

	UPROPERTY(EditAnywhere)
	float SideMoveSpeed = 450.0f;

	// 行为参数
	UPROPERTY(EditAnywhere)
	float SideMoveDuration = 2.5f;

	UPROPERTY(EditAnywhere)
	float DirectionChangeProb = 0.3f;
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

	// 触发条件
	UPROPERTY(EditAnywhere, Category = "Condition")
	FGameplayTagQuery ActivationQuery;

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

protected:
	virtual void BeginPlay() override;

	//样条线组件指针，从Owner获取
	UPROPERTY(BlueprintReadWrite, Category = "AI|Patrol")
	TObjectPtr<USplineComponent> PatrolSpline;

	// 巡逻参数
	UPROPERTY(EditAnywhere, Category = "AI|Patrol")
	FPatrolConfig PatrolConfig;

	//巡逻行为树
	UPROPERTY(EditAnywhere, Category = "AI|Patrol")
	TObjectPtr<UBehaviorTree> PatrolSubTree;

	//对峙参数
	UPROPERTY(EditAnywhere, Category = "AI|Confrontation")
	FConfrontationConfig ConfrontationConfig;

	//对峙行为树
	UPROPERTY(EditAnywhere, Category = "AI|Confrontation")
	TObjectPtr<UBehaviorTree> ConfrontationSubTree;

	// 配置数据引用
	UPROPERTY(EditAnywhere, Category = "AI|Attack")
	TObjectPtr<UDataTable> SkillConfigTable;

public:	
	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE UBehaviorTree* GetPatrolSubTree() const { return PatrolSubTree; }
		
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TArray<FVector>& GetPatrolPoints() { return PatrolPoints; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FPatrolConfig& GetPatrolConfig() { return PatrolConfig; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UBehaviorTree* GetConfrontationSubTree() const { return ConfrontationSubTree; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FConfrontationConfig& GetConfrontationConfig() { return ConfrontationConfig; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UDataTable* GetSkillConfigTable() { return SkillConfigTable; }
};
