// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h" 
#include "NiagaraSystem.h"                 // ���� Niagara ϵͳͷ�ļ�
#include "NiagaraFunctionLibrary.h"        // ���� Niagara ���ܿ⣬���ڲ�����Ч
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

	//����
	void Fire(FVector Direction, float Strength);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//�ص��¼�
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlapedComp,AActor* OtherActor,UPrimitiveComponent* OtherComponent,int OtherBodyIndex,bool bFromSweep,const FHitResult& HitResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* ArrowStaitcMesh;

	// ��������˶����
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* NiagaraAttachComponent;

	//����ٶ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = Arrow)
	float MaxSpeed;

	//��С�ٶ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Arrow)
	float MinSpeed;

	//�������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Arrow)
	float MaxGravity;

	//��С����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Arrow)
	float MinGravity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Arrow)
	USoundCue* ArrowShootSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Arrow)
	USoundCue* ArrowHitSound;

	// ��������ϵͳ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Arrow)
	UParticleSystem* ParticleEffect;  // ����ϵͳ��ָ��

	// ���� Niagara ϵͳ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	UNiagaraSystem* NiagaraEffect; // Niagara ϵͳ��ָ��

	//��ʸ��ǰ���ٶ�
	FVector ArrowVelocity;
protected:

//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;

};
