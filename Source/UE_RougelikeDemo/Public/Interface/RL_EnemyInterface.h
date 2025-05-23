// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RL_EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URL_EnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class UAS_Enemy;
/**
 * 
 */
class UE_ROUGELIKEDEMO_API IRL_EnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetCombatTarget() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetLockTarget(bool bInLock);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetLockUIRed(bool bInRedLock);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UNiagaraComponent* GetRedAttackNiagaraComponent() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetHealthBarVisible(bool bVisible) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetHitShake(FName BoneName, FVector ShakeDirection, float Magnitude);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAS_Enemy* GetEnemyAttributeSet() const;
};
