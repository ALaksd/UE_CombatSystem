// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include <GameplayTagContainer.h>
#include "ANS_EnemyAttackDecision.generated.h"


class UGameplayEffect;
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

	// 攻击检测范围
	UPROPERTY(EditAnywhere, Category = "Detection")
	float AttackRadius = 100.0f;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	TArray<AActor*> AlreadyHitActors; // 已命中的目标
	AActor* OwnerActor; // 攻击者

	void CauseDamage(AActor* TargetActor);
	void DetectAndApplyDamage(USkeletalMeshComponent* MeshComp);

};
