// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include <GameplayTagContainer.h>
#include "ANS_EnemyAttackDecision.generated.h"


class UGameplayEffect;
class URL_EnemyMovementComponent;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UANS_EnemyAttackDecision : public UAnimNotifyState
{
	GENERATED_BODY()
	

public:
	// 伤害配置
	UPROPERTY(EditAnywhere, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, Category = "Damage")
	FGameplayTag DamageTypeTag;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage = 20.0f;

	//减少理智的量 = Damage * ReduceSantiyFactor
	UPROPERTY(EditAnywhere, Category = "Damage")
	float ReduceSantiyFactor = 0.5f;

	//击退距离，击退的力 = 敌人当时的朝向 * 击退距离
	UPROPERTY(EditAnywhere, Category = "Damage")
	float KnockDistance = 150.f;

	// 攻击检测范围
	UPROPERTY(EditAnywhere, Category = "Detection")
	FVector RectangleParam = FVector(20, 20, 60); // 前向50，横向50，垂直50

	UPROPERTY(EditAnywhere, Category = "Detection")
	FVector LocationOffset = FVector::ZeroVector; // 相对于骨骼的局部偏移

	UPROPERTY(EditAnywhere, Category = "Detection")
	FName AttackSocketName = "AttackSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Parry")
	TSubclassOf<UGameplayEffect> ParryAttributeEffect;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	TArray<AActor*> AlreadyHitActors; // 已命中的目标

	UPROPERTY()
	AActor* OwnerActor; // 攻击者

	void CauseDamage(AActor* TargetActor);
	void DetectAndApplyDamage(USkeletalMeshComponent* MeshComp, FVector& Center, FRotator& Rotation);

	// 攻击检测范围
	FTransform SocketTrans;
};
