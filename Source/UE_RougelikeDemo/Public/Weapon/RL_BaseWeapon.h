// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "GAS/AS/AS_Weapon.h"
#include "RL_BaseWeapon.generated.h"

UCLASS()
class UE_ROUGELIKEDEMO_API ARL_BaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ARL_BaseWeapon();

	UPROPERTY()
	TObjectPtr<AActor> WeaponOwner;
	
	//武器等级
	int32 WeaponLevel=1;


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly , Category = Mesh)
	class USkeletalMeshComponent* Mesh;


	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> WeaponASC;
	UPROPERTY()
	TObjectPtr<UAS_Weapon> WeaponAttribute;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffet;
	
	//武器伤害曲线
	// UPROPERTY(EditDefaultsOnly,Category="AttackCurve")
	// TSubclassOf<UGameplayEffect> DamageCurve;
	
	bool bCombat = false;

	//储存当前帧插槽位置
	TArray<FVector> CurrentPoints;
	//储存上一帧插槽位置
	TArray<FVector> LastPoints;

	//碰撞忽略的Actor，避免自己受伤
	UPROPERTY(EditDefaultsOnly,Category="Attack Collision")
	TArray<AActor*> ActorsToIgnore;

	//射线检测碰到的Actor，用于避免二次受伤
	UPROPERTY()
	TArray<AActor*> HitActors;
	//要执行的碰撞检测对象，填Enemy
	UPROPERTY(EditDefaultsOnly,Category="Attack Collision")
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;

	FGameplayEffectSpecHandle DamageSpecHandle;
public:
	void StartCombat();
	void EndCombat();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	void GetCurrentPointsLocation();
	//将当前帧位置给到下一帧
	void SetLastPointsLocation();

};
