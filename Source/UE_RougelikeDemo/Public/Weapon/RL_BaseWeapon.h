// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "GAS/AS/AS_Weapon.h"
#include "Interface/RL_ItemInstanceHolder.h"
#include "RL_BaseWeapon.generated.h"

UENUM(BlueprintType)
enum class WeaponType : uint8
{
	Sword,          // 长剑
	GreatSword,     // 大剑
};

UCLASS()
class UE_ROUGELIKEDEMO_API ARL_BaseWeapon : public AActor, public IRL_ItemInstanceHolder
{
	GENERATED_BODY()
	
public:	
	ARL_BaseWeapon();

  	FORCEINLINE AActor* GetWeaponOwner() const { return WeaponOwner; }
	FORCEINLINE void SetWeaponOwner(AActor* InOwner) { WeaponOwner = InOwner; }
	void SetWeaponLevel(int32 NewLevel);

	//武器种类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	WeaponType Type;

	//武器击退距离
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float KnockDistance = 100.f;

	/**
	 * ItemInstanceHolder Interface
	 */

	 // 实现接口方法
	virtual URLInventoryItemInstance* GetItemInstance_Implementation() const override { return ItemInstance; }
	virtual void SetItemInstance_Implementation(URLInventoryItemInstance* NewInstance) override { ItemInstance = NewInstance; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Mesh")
	class USkeletalMeshComponent* Mesh;

	//要执行的碰撞检测对象，填Enemy
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Attack Collision")
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;

	//单次攻击恢复的信仰值，会乘以武器当前的攻击倍率
	UPROPERTY(EditDefaultsOnly)
	float RestoreAttach = 0.2f;

	//单次攻击恢复的信理智值，会乘以武器当前的攻击倍率
	UPROPERTY(EditDefaultsOnly)
	float RestoreSanity = 5.f;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> WeaponASC;
	UPROPERTY()
	TObjectPtr<UAS_Weapon> WeaponAttribute;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffet;

	UPROPERTY()
	TObjectPtr<AActor> WeaponOwner;

	//武器等级
	UPROPERTY()
	int32 WeaponLevel = 0;
	
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

	void RestoreAttachResourceAndSanity(float DamageMultiplier);

private:
	void GetCurrentPointsLocation();
	//将当前帧位置给到下一帧
	void SetLastPointsLocation();

};
