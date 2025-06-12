// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RL_EnemySpawnVolumes.generated.h"

class UBoxComponent;
class ARL_EnemySpawnPoint;

UCLASS()
class UE_ROUGELIKEDEMO_API ARL_EnemySpawnVolumes : public AActor
{
	GENERATED_BODY()
	
public:
	ARL_EnemySpawnVolumes();

	void ResetEnemySpawn();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlapedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere)
	TArray<ARL_EnemySpawnPoint*> SpawnPoints;

	//是否已生成敌人，传送后应该重置
	UPROPERTY(BlueprintReadWrite)
	bool bSpawned = false;
};
