// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RL_CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URL_CombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_ROUGELIKEDEMO_API IRL_CombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMotange();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetLightHitReactFrontMontage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetLightHitReactBackMontage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetLightHitReactLeftMontage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetLightHitReactRightMontage() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHeavyHitReactMontage() const;


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFacingTarget(FVector TargetLoction);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool isDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void KnockBack(const FVector& KonckBackImpulse);
};
