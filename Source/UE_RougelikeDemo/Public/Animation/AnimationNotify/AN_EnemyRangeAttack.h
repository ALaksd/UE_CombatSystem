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
	UPROPERTY(EditAnywhere)
	FRangeDamageParams RangeDamageParams;

	UPROPERTY(EditAnywhere)
	FName SocketName;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> AttackActorClass;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
