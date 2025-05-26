// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/RL_BaseWeapon.h"
#include "RL_Sword.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API ARL_Sword : public ARL_BaseWeapon
{
	GENERATED_BODY()

public:
	ARL_Sword();

	void StartCombat();
	void EndCombat();

protected:
	void RestoreAttachResourceAndSanity(float DamageMultiplier);

private:
	void GetCurrentPointsLocation();
	//将当前帧位置给到下一帧
	void SetLastPointsLocation();

	virtual void Tick(float DeltaTime) override;
	
protected:
	
	//武器击退距离
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float KnockDistance = 100.f;
	
	//单次攻击恢复的信仰值，会乘以武器当前的攻击倍率
	UPROPERTY(EditDefaultsOnly)
	float RestoreAttach = 0.2f;

	//单次攻击恢复的信理智值，会乘以武器当前的攻击倍率
	UPROPERTY(EditDefaultsOnly)
	float RestoreSanity = 5.f;

	
private:
	bool bCombat = false;	

	//储存当前帧插槽位置
	TArray<FVector> CurrentPoints;
	//储存上一帧插槽位置
	TArray<FVector> LastPoints;

	//射线检测碰到的Actor，用于避免二次受伤
	UPROPERTY()
	TArray<AActor*> HitActors;
};
