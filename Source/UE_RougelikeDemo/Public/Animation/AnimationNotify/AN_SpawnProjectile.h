// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SpawnProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UAN_SpawnProjectile : public UAnimNotify
{
	GENERATED_BODY()
public:
	// 伤害数值
	UPROPERTY(EditAnywhere,Category="Attribute")
	float Damage;

	// Damage的类型,标识是否是红光攻击
	UPROPERTY(EditAnywhere,Category="Attribute")
	FGameplayTag Tag;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	
};
