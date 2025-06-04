// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include <GameplayTagContainer.h>
#include <Data/Enums.h>
#include "ANS_EnemyAttackDecision.generated.h"



class UGameplayEffect;
class UAbilitySystemComponent;
class URL_EnemyMovementComponent;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UANS_EnemyAttackDecision : public UAnimNotifyState
{
	GENERATED_BODY()
	

public:
	UANS_EnemyAttackDecision();

	// 伤害配置
	UPROPERTY(EditAnywhere, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, Category = "Damage")
	FGameplayTag DamageTypeTag;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage = 20.0f;

	//弹反成功敌人减少的体力条
	UPROPERTY(EditAnywhere, Category = "Damage")
	float Breakingvalue = 40.0f;

	//弹反成功回复的理智值
	UPROPERTY(EditAnywhere, Category = "Damage")
	float RestoreSanity = 40.0f;

	//减少理智的量 = Damage * ReduceSantiyFactor
	UPROPERTY(EditAnywhere, Category = "Damage")
	float ReduceSantiyFactor = 0.5f;

	//击退距离，击退的力 = 敌人当时的朝向 * 击退距离
	UPROPERTY(EditAnywhere, Category = "Damage")
	float KnockDistance = 150.f;

	// 检测形状类型（矩形/球形）
	UPROPERTY(EditAnywhere, Category = "Detection")
	EDetectionShapeType DetectionShape = EDetectionShapeType::Rectangle;

	// 矩形参数：长宽高（X:前后 Y:左右 Z:上下）
	UPROPERTY(EditAnywhere, Category = "Detection", meta = (EditCondition = "DetectionShape == EDetectionShapeType::Rectangle"))
	FVector RectangleParam = FVector(20, 20, 60);

	// 球形参数：半径
	UPROPERTY(EditAnywhere, Category = "Detection", meta = (EditCondition = "DetectionShape == EDetectionShapeType::Sphere"))
	float SphereRadius = 80.f;


	UPROPERTY(EditAnywhere, Category = "Detection")
	FVector LocationOffset = FVector::ZeroVector; // 相对于骨骼的局部偏移

	UPROPERTY(EditAnywhere, Category = "Detection")
	FName AttackSocketName = "AttackSocket";

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> AttackSound;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	TArray<AActor*> AlreadyHitActors; // 已命中的目标

	UPROPERTY()
	AActor* OwnerActor; // 攻击者

	void CauseDamage(AActor* TargetActor,FVector HitLoction,FVector HitNormal);
	bool ParryDecision(UAbilitySystemComponent* TargetASC, UAbilitySystemComponent* SourceASC, FVector& HitLoction, FVector& HitNormal, AActor* TargetActor);
	void DetectAndApplyDamage(USkeletalMeshComponent* MeshComp, FVector& Center, FRotator& Rotation);

	// 攻击检测范围
	FTransform SocketTrans;
};
