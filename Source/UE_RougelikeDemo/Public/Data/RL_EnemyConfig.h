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

UENUM(BlueprintType)
enum class EPositionType : uint8
{
	Anywhere  UMETA(DisplayName = "任意位置"),
	Front     UMETA(DisplayName = "前方"),
	Back      UMETA(DisplayName = "后方"),
	Left      UMETA(DisplayName = "左侧"),
	Right     UMETA(DisplayName = "右侧")
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


	// 受击动画
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|AnimMontage|Hurt")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|AnimMontage|Hurt")
	TObjectPtr<UAnimMontage> HitReactFrontMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|AnimMontage|Hurt")
	TObjectPtr<UAnimMontage> HitReactBackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|AnimMontage|Hurt")
	TObjectPtr<UAnimMontage> HitReactRightMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|AnimMontage|Hurt")
	TObjectPtr<UAnimMontage> HitReactFLeftMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|AnimMontage|Hurt")
	TObjectPtr<UAnimMontage> HitReactHeavyFrontMontage;

	// 正面处决动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|AnimMontage|Execute")
	TObjectPtr<UAnimMontage> Aim_Execute_F;
	// 背面处决动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy|AnimMontage|Execute")
	TObjectPtr<UAnimMontage> Aim_Execute_B;


	//特殊受击动画(被弹反)
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|AnimMontage")
	TObjectPtr<UAnimMontage> ParryHitMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Weapon")
	TObjectPtr<UStaticMesh> WeaponSeletakMesh;


	//敌人手持武器插槽
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Weapon")
	FName WeaponAttachSocket = "WeaponSocket";

	//武器顶端插槽
	UPROPERTY(EditDefaultsOnly, Category = "Enemy|Weapon")
	FName WeaponTipSocket = "Tip";

	// 敌人类型等扩展参数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	FString EnemyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 EnemyLevel = 1;
};
