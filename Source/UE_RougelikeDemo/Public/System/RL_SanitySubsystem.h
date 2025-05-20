// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/Enums.h"
#include "RL_SanitySubsystem.generated.h"

// 广播理智状态变化
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSanityStateChanged, E_SanityState, ESanityState,float, CurrentSanity);

/**
 * 理智系统
 */

UCLASS()
class UE_ROUGELIKEDEMO_API URL_SanitySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/*-------------------Getter/Setter---------------------*/
	inline float GetSanity() const{return CurrentSanity;}
	inline float GetSanityPercent() const {return CurrentSanity/MaxSanity;}
	UFUNCTION(BlueprintCallable, Category = "Sanity System|Getter")
	inline E_SanityState GetSanityState() const{return SanityState;}
	/*-------------------Getter/Setter---------------------*/

	// 削减理智
	UFUNCTION(BlueprintCallable)
	void ReduceSanity(float Sanity);
	// 回复理智
	UFUNCTION(BlueprintCallable)
	void RestoreSanity(float Amount);

	// 回复理智到最大值
	void RestoreSanityToMax();

	UFUNCTION(BlueprintCallable, Category = "Sanity System")
	void SetCombatState(bool bInCombat);

	
	// 委托
	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnSanityStateChanged OnSanityStateChanged;

protected:
	/** 理智减少间隔 */
	UPROPERTY(EditDefaultsOnly)
	float Interval = 0.5f;

	/** 每间隔减少理智数量 */
	UPROPERTY(EditDefaultsOnly)
	float DeltaAmount = 0.5f;
	
private:
	// 最大理智值
	float MaxSanity;
	// 当前理智值
	float CurrentSanity;

	// 当前理智状态
	E_SanityState SanityState;

	// 初始化
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;


	FTimerHandle AutoReduceSanityTimer;
	bool bIsInCombat = false;

	void ReduceSanityOverTime();
};
