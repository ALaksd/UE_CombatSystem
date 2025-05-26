// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "GAS/AS/AS_Weapon.h"
#include "Interface/RL_ItemInstanceHolder.h"
#include "RL_BaseWeapon.generated.h"

class USkeletalMeshComponent;

UENUM(BlueprintType)
enum class E_WeaponType : uint8
{
	Sword,          // 长剑
	GreatSword,     // 大剑
	Bow,			// 弓
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
	E_WeaponType WeaponType;

	/**
	 * ItemInstanceHolder Interface
	 */

	 // 实现接口方法
	virtual URLInventoryItemInstance* GetItemInstance_Implementation() const override { return ItemInstance; }
	virtual void SetItemInstance_Implementation(URLInventoryItemInstance* NewInstance) override { ItemInstance = NewInstance; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Mesh")
	USkeletalMeshComponent* Mesh;

	//要执行的碰撞检测对象
	UPROPERTY(EditDefaultsOnly, Category = "Weapon|Attack Collision")
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> WeaponASC;
	UPROPERTY()
	TObjectPtr<UAS_Weapon> WeaponAttribute;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DamageEffet;

	//武器等级
	UPROPERTY()
	int32 WeaponLevel = 0;

	UPROPERTY()
	TObjectPtr<AActor> WeaponOwner;

	FGameplayEffectSpecHandle DamageSpecHandle;

	/** 物品实例 */
	UPROPERTY()
	TObjectPtr<URLInventoryItemInstance> ItemInstance = nullptr;
};
