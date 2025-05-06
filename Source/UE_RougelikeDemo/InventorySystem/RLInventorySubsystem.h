// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RLInventorySubsystem.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URLInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	URLInventoryItemInstance* GenerateItemInstance(URLInventoryItemDefinition *ItemDefinition);


	//生成ItemIntance的物理表示，可用于在World生成可拾取物品，附加到角色身上等
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AItem_Pickup* SpawnItemActorFromInstance(URLInventoryItemInstance* ItemInstance, const FVector& Location);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AActor* SpawnItemActorFromDefinition(URLInventoryItemDefinition* ItemDefinition, const FVector& Location);

	/**
	* 升级武器并消耗材料
	* @param WeaponInstance 要升级的武器实例
	* @param OwnerInventory 玩家库存组件
	* @return 是否升级成功
   */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Upgrade")
	bool UpgradeWeapon(URLInventoryItemInstance* WeaponInstance,URLInventoryComponent* OwnerInventory);

	UFUNCTION(BlueprintCallable, Category = "Inventory|Upgrade")
	FWeaponLevelData GetCurrentWeaponLevelData(URLInventoryItemInstance* WeaponInstance);

	/** 获取背包里的指定武器强化碎片的数量 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Upgrade")
	int32 GetCurrentWeaponFragmentNum(URLInventoryComponent* OwnerInventory,FGameplayTag ItemTag);

	/** 获取当前灵魂 */
	UFUNCTION(BlueprintCallable, Category = "Inventory|Upgrade")
	int32 GetCurrentSoul(URLInventoryComponent* OwnerInventory);

};
