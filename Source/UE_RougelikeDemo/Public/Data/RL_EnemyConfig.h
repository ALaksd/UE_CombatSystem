// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include <Abilities/GameplayAbility.h>
#include "RL_EnemyConfig.generated.h"


USTRUCT(BlueprintType)
struct FEnemySkillAnimation
{
	GENERATED_BODY()

	/** 动画资源 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<UAnimMontage> Montage;

	/** 允许的状态（清醒/混沌） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FGameplayTagContainer AllowedStates;

	/** 动画选中权重 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float Weight = 1.0f;

};

USTRUCT(BlueprintType)
struct FEnemySkills
{
	GENERATED_BODY()

	/** 能力标签*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FGameplayTag AbilityTag;

	/** 能力使用的类，如播放动画攻击类、投射物类等 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TSubclassOf<class UGA_Base> AbilityClass;

	/** 优先级 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	int32 PriorityLevel = 0;

	/** 是否为强力攻击 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	bool bIsPowerfulAttack = false;


	/** 攻击范围 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float SkillRangeMin = 50.f;

	/** 攻击范围 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	float SkillRangeMax = 300.f;

	/** 所有动画条目配置 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	TArray<FEnemySkillAnimation> Animations;
};

UENUM(BlueprintType)
enum class EEnemyActionState : uint8
{
	None        UMETA(DisplayName = "无状态"),
	Evading     UMETA(DisplayName = "迂回中"),
	Rolling     UMETA(DisplayName = "翻滚中"),
	Attacking   UMETA(DisplayName = "攻击中")
};

USTRUCT(BlueprintType)
struct FBaseActionWeights
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float EvadeChance = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float AttackChance = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float RollChance = 0.1f;
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
	TArray<FEnemySkills> EnemySkills;

	FEnemySkills FindSkillsByTag(FGameplayTag SkillTag) const;

	// 初始属性
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Attributes")
	TSubclassOf<UGameplayEffect> PrimariAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|AnimMontage")
	TObjectPtr<UAnimMontage> HitReactMontage;


	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Weapon")
	TObjectPtr<UStaticMesh> WeaponSeletakMesh;


	//敌人手持武器插槽
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Weapon")
	FName WeaponAttachSocket = "WeaponSocket";

	//武器顶端插槽
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Weapon")
	FName WeaponTipSocket = "Tip";

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|AI")
	FBaseActionWeights BaseActionWeights;

	// 敌人类型等扩展参数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	FString EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 EnemyLevel = 1;
};
