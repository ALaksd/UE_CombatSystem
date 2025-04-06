// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h" 
#include "NiagaraSystem.h"                 // 引入 Niagara 系统头文件
#include "NiagaraFunctionLibrary.h"        // 引入 Niagara 功能库，用于播放特效
#include "NiagaraTypes.h"
#include "NiagaraCore.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RL_Arrow.generated.h"

UCLASS()
class UE_ROUGELIKEDEMO_API ARL_Arrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARL_Arrow();

	//发射
	void Fire(FVector Direction, float Strength);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//重叠事件
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlapedComp,AActor* OtherActor,UPrimitiveComponent* OtherComponent,int OtherBodyIndex,bool bFromSweep,const FHitResult& HitResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* ArrowStaitcMesh;

	// 添加抛体运动组件
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* NiagaraAttachComponent;

	//最大速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = Arrow)
	float MaxSpeed;

	//最小速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Arrow)
	float MinSpeed;

	//最大重力
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Arrow)
	float MaxGravity;

	//最小重力
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Arrow)
	float MinGravity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Arrow)
	USoundCue* ArrowShootSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Arrow)
	USoundCue* ArrowHitSound;

	// 声明粒子系统
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Arrow)
	UParticleSystem* ParticleEffect;  // 粒子系统的指针

	// 声明 Niagara 系统
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	UNiagaraSystem* NiagaraEffect; // Niagara 系统的指针

	//箭矢当前的速度
	FVector ArrowVelocity;
protected:

//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

};
