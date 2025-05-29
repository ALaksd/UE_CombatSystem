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
	// 中心偏移
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute | Arc Settings")
	FVector ArcCenterOffset = FVector::ZeroVector;
	
	// 圆弧半径
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute | Arc Settings")
	float ArcRadius = 500.0f; 

	// 角度范围
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute | Arc Settings")
	float ArcAngleDegrees  = 500.0f;

	
	// 是否是火球
	UPROPERTY(EditAnywhere,Category="Attribute")
	bool bIsFireBall;
	
	// 生成的发射物数量,弓箭填1
	UPROPERTY(EditAnywhere,Category="Attribute")
	float Count;
	
	// 伤害数值
	UPROPERTY(EditAnywhere,Category="Attribute")
	float Damage;

	// Damage的类型,标识是否是红光攻击
	UPROPERTY(EditAnywhere,Category="Attribute")
	FGameplayTag Tag;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
	
};
