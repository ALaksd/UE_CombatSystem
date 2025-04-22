// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/RL_ItemDataAsset.h"
#include "RL_InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID = FName();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URL_ItemDataAsset* ItemData = nullptr;

	// 单件物品可能有唯一 ID（例如生成的武器/装备）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid UniqueID = FGuid();
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API URL_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URL_InventoryComponent();

	/**
	 * 背包接口
	 */
	/*Get*/
	FORCEINLINE TArray<FInventorySlot> GetAllWeapons()const { return WeaponItems; }
	FORCEINLINE TArray<FInventorySlot> GetAllEquipment()const { return EquipmentItems; }
	FORCEINLINE TMap<FName, FInventorySlot> GetAllUseItems()const { return UseItems; }

	/*Add*/
	void AddItem(URL_ItemDataAsset* ItemData, int32 Count);

protected:
	virtual void BeginPlay() override;

	/**
	 * 背包数据
	 */

	/*武器*/
	TArray<FInventorySlot> WeaponItems;
	/*装备*/
	UPROPERTY()
	TArray<FInventorySlot> EquipmentItems;
	/*道具*/
	UPROPERTY()
	TMap<FName, FInventorySlot> UseItems;
};
