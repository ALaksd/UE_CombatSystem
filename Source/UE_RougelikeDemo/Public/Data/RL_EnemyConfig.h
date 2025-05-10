// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include <Abilities/GameplayAbility.h>
#include "RL_EnemyConfig.generated.h"


USTRUCT(BlueprintType)
struct FEnemyAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxResilience = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WalkSpeed = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RunSpeed = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SightRange = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HearingRange = 1200.f;
};

USTRUCT(BlueprintType)
struct FEnemySkills
{
	GENERATED_BODY()

	/** 技能对应的 GameplayTag，用于行为树或条件判断 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FGameplayTag AbilityTag;

	/** 是否是红光攻击（可用于视觉反馈或逻辑区分） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	bool bRed = false;

	/** 技能在被选中时的概率权重，权重大说明更容易被选中 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float SelectionWeight = 1.0f;

	/** 技能的优先级，数值越大越优先执行 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	int32 PriorityLevel = 0;

	/** 技能冷却时间（秒） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float Cooldown = 5.f;

	/** 技能类，必须继承自 UGameplayAbility */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSubclassOf<class UGameplayAbility> Skill;

	/** 技能使用时的动画 Montage，可用于播放动作 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UAnimMontage> SkillMontage;
};


/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_EnemyConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	// 技能
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Skills")
	TArray<FEnemySkills> WakingStateAttackSkills;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Skills")
	TArray<FEnemySkills> ChaosChaosAttackSkills;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Skills")
	TArray<FEnemySkills> OtherSkills;

	// 初始属性
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Attributes")
	FEnemyAttributes Attributes;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|AnimMontage")
	TObjectPtr<UAnimMontage> HitReactMontage;


	// 敌人类型等扩展参数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	FString EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 EnemyLevel = 1;
};
