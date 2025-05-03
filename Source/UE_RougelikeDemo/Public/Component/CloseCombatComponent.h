// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/RL_BaseWeapon.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryComponent.h"
#include "CloseCombatComponent.generated.h"

class URLInventoryComponent_Equipment;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API UCloseCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCloseCombatComponent();

	/*Getter*/
	UFUNCTION(BlueprintCallable,BlueprintPure)
	FORCEINLINE ACharacter* GetUser() const { return User; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE ARL_BaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE URLInventoryComponent_Equipment* GetEquipmentInventoryComponent() const { return EquipmentInventoryComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<ACharacter> User = nullptr;

	/** 主武器 */
	UPROPERTY()
	TObjectPtr<ARL_BaseWeapon> CurrentWeapon;

	/** 当前选中的武器槽位索引 */
	UPROPERTY()
	int32 CurrentWeaponIndex = 0;

	/** 武器对象池 */
	UPROPERTY()
	TArray<TObjectPtr<ARL_BaseWeapon>> WeaponPool;


	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URLInventoryComponent_Equipment> EquipmentInventoryComponent;


public:	
	void StartCombat() const;
	void EndCombat() const;
	UFUNCTION()
	void OnEquipSlotUpdate(URLInventoryComponent* InventoryComponent,const FRLInventoryItemSlotHandle& SlotHandle, URLInventoryItemInstance* ItemInstance, URLInventoryItemInstance* PreviousItemInstance);

	/*从背包装备/卸下/切换武器*/
	void EquipWeaponForInventory(URLInventoryItemInstance* ItemInstance);
	void UnEquipWeaponForInventory();

	UFUNCTION(BlueprintCallable)
	void SwitchWeapon();

	/** 预加载武器 */
	void PreloadWeapons();

	/** 根据实例获取武器对象 */
	ARL_BaseWeapon* GetWeaponFromInstance(URLInventoryItemInstance* Instance);
};
