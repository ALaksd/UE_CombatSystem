// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/RL_BaseWeapon.h"
#include "RL_Sword.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API ARL_Sword : public ARL_BaseWeapon
{
	GENERATED_BODY()

public:
	ARL_Sword();

	void StartCombat(float StaminaReduce_T,float ResilienceReduce_T);
	void EndCombat();

	//刀光特效
	UFUNCTION()
	void StartTrailEffect();

	UFUNCTION()
	void StopTrailEffect();

	UFUNCTION()
	void EnableFireRant(float ActiveTime);

protected:
	void RestoreAttachResourceAndSanity(float DamageMultiplier);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UNiagaraComponent* TrailComponent;

	// 火焰附魔特效
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UNiagaraComponent* FireRantComponent;

	// 火焰附魔拖尾特效
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UNiagaraComponent* FireTrailComponent;

	// 是否启用火焰附魔
	bool bIsFireRant;
	
private:
	void GetCurrentPointsLocation();
	//将当前帧位置给到下一帧
	void SetLastPointsLocation();

	virtual void Tick(float DeltaTime) override;

	// 体力削减
	float StaminaReduce;

	// 韧性削减
	float ResilienceReduce;

	UGameplayEffect* CreateReduceGE(float Stamina,float Resilience);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|Mesh")
	USkeletalMeshComponent* Mesh;
	
	//武器击退距离
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Attack")
	float KnockDistance = 100.f;
	
	//单次攻击恢复的信仰值，会乘以武器当前的攻击倍率
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Attack")
	float RestoreAttach = 0.2f;

	//单次攻击恢复的信理智值，会乘以武器当前的攻击倍率
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Attack")
	float RestoreSanity = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Sound")
	USoundBase* AttackSound;
	
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
