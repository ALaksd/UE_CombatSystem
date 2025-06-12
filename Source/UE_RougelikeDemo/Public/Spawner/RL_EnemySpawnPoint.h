// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "RL_EnemySpawnPoint.generated.h"

class AEnemy_Base;
class USplineComponent;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API ARL_EnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:

	ARL_EnemySpawnPoint();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<AEnemy_Base> EnemyClass;
	
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UFUNCTION(BlueprintCallable)
	void DestroyEnemy();

	//样条线组件，用于巡逻
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> PatrolSpline;

private:
	UPROPERTY()
	AEnemy_Base* CurrentSpawndEnemy;

	UPROPERTY()
	AEnemy_Base* LastSpawndEnemy;
};
