// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RL_Bow.generated.h"


enum class E_WeaponType : uint8;
class UCapsuleComponent;
class ARL_ProjectileBase;

UENUM(BlueprintType)
enum class EBowState : uint8
{
	Idle,
	Aim,
	Draw, 
};

class UAudioComponent;

UCLASS()
class UE_ROUGELIKEDEMO_API ARL_Bow : public AActor
{
	GENERATED_BODY()

public:
	ARL_Bow();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	EBowState BowState;

	//播放拉弓声音
	void PullBowStart();
	//停止播放拉弓声音
	void PullBowEnd();

	
	//最大拉弓时间
	float MaxPullTime = 1.0f;

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMesh(){return SkeletalMeshComponent;}
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	// 箭矢
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Arrow")
	TSubclassOf<ARL_ProjectileBase> ArrowClass;

	// 箭矢生成位置插槽
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Arrow | Attribute")
	FName SpawnSocke;
	
	// 发射箭矢
	/// 
	/// @param AimLocation 目标位置
	void FireProjectile(FVector AimLocation);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Audio")
	USoundCue* BowDrawSound;


	E_WeaponType WeaponType;




	UPROPERTY()
	UAudioComponent* SoundToPlay;
};
