// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Data/Structs.h"
#include "ANS_SpawnProjectile.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UANS_SpawnProjectile : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	// 是否显示生成位置
	UPROPERTY(EditAnywhere,Category="Attribute | Settings")
	bool bIsShowDebug;
	// 是否是火球
	UPROPERTY(EditAnywhere,Category="Attribute | Settings")
	bool bIsFireBall;
	
	// 伤害数值
	UPROPERTY(EditAnywhere,Category="Attribute")
	float Damage;

	// Damage的类型,标识是否是红光攻击
	UPROPERTY(EditAnywhere,Category="Attribute")
	FGameplayTag Tag;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	FName ArrowSpawnSocket;

	// 火球生成位置,旋转
	UPROPERTY(EditAnywhere,Category="Attribute | Fireball")
	TArray<FFirebalLocation> Locations;

	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;


private:
	void ShowFireballLocation(AActor* Owner);
	
};
