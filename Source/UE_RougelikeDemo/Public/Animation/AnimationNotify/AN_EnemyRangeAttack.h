// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GAS/RL_AbilitySystemLibrary.h"
#include "AN_EnemyRangeAttack.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UAN_EnemyRangeAttack : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Attack|Effect")
	UNiagaraSystem* NiagaraEffect;

	UPROPERTY(EditAnywhere, Category = "Attack|Location")
	FName SocketName;

	UPROPERTY(EditAnywhere, Category = "Attack|Location")
	float SphereRadius = 100.f;

	UPROPERTY(EditAnywhere, Category = "Attack|Damage")
	FDamageParams DamageParams;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> AttackActorClass;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
