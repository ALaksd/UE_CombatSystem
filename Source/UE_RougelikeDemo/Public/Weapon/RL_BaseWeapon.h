// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "GAS/AS/AS_Weapon.h"
#include "Interface/RL_ItemInstanceHolder.h"
#include "RL_BaseWeapon.generated.h"


UCLASS()
class UE_ROUGELIKEDEMO_API ARL_BaseWeapon : public AActor, public IRL_ItemInstanceHolder
{
	GENERATED_BODY()
	
public:	
	ARL_BaseWeapon();

  	FORCEINLINE AActor* GetWeaponOwner() const { return WeaponOwner; }
	FORCEINLINE void SetWeaponOwner(AActor* InOwner) { WeaponOwner = InOwner; }
	//武器等级
	int32 WeaponLevel=1;

	/**
	 * ItemInstanceHolder Interface
	 */

	 // 实现接口方法
	virtual URLInventoryItemInstance* GetItemInstance_Implementation() const override { return ItemInstance; }
	virtual void SetItemInstance_Implementation(URLInventoryItemInstance* NewInstance) override { ItemInstance = NewInstance; }

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

	UPROPERTY()
	TObjectPtr<AActor> WeaponOwner;
	
	//武器伤害曲线
	// UPROPERTY(EditDefaultsOnly,Category="AttackCurve")
	// TSubclassOf<UGameplayEffect> DamageCurve;
	
	bool bCombat = false;	

	//储存当前帧插槽位置
	TArray<FVector> CurrentPoints;
	//储存上一帧插槽位置
	TArray<FVector> LastPoints;
	

	//射线检测碰到的Actor，用于避免二次受伤
	UPROPERTY()
	TArray<AActor*> HitActors;
	//要执行的碰撞检测对象，填Enemy
	UPROPERTY(EditDefaultsOnly,Category="Attack Collision")
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;

	FGameplayEffectSpecHandle DamageSpecHandle;

	/** 物品实例 */
	UPROPERTY()
	TObjectPtr<URLInventoryItemInstance> ItemInstance = nullptr;

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
