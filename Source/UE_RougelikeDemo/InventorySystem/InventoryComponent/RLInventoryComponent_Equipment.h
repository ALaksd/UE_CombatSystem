// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/InventorySystem//RLInventoryComponent.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemFragment.h"
#include "RLInventoryComponent_Equipment.generated.h"


USTRUCT(BlueprintType)
struct UE_ROUGELIKEDEMO_API FRLInventoryItemInfoEntry
{
	GENERATED_BODY()

public:
	FRLInventoryItemInfoEntry() {}

	FRLInventoryItemInfoEntry(const FRLInventoryItemSlotHandle& InHandle)
		: Handle(InHandle) {}

	UPROPERTY(BlueprintReadOnly)
	FRLInventoryItemSlotHandle Handle;

	UPROPERTY(BlueprintReadOnly)
	FRLAbilitySet_GrantHandles AbilityGrantHandles;

	bool operator==(const FRLInventoryItemInfoEntry& InEntry) const {
		return this->Handle == InEntry.Handle;
	}

	bool operator==(const FRLInventoryItemInfoEntry* InEntry) const {
		return this->Handle == InEntry->Handle;
	}
};

/** 装备槽位 */
USTRUCT(BlueprintType)
struct FEquipmentSlotGroup
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	FGameplayTag SlotTypeTag; // 主类型Tag 如 Equipment.Type.Weapon

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	int32 SlotCount = 1; // 该类型下的槽位数量
};

USTRUCT()
struct FEquipWeapon
{
	GENERATED_BODY()
	UPROPERTY()
	URLInventoryItemInstance* ItemInstance;
	FRLInventoryItemSlotHandle Handle;
};

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipUpdate,URLInventoryItemInstance*, NewItem,URLInventoryItemInstance*, OldItem);

UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE_ROUGELIKEDEMO_API URLInventoryComponent_Equipment : public URLInventoryComponent
{
	GENERATED_BODY()

public:
	URLInventoryComponent_Equipment(const FObjectInitializer& ObjectInitializer);
	virtual void InitializeComponent() override;


	/** 查询接口 */

	// 获取指定类型的槽位
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	TArray<FRLInventoryItemSlotHandle> GetSlotsByType(FGameplayTag SlotTypeTag) const;

	// 获取当前装备的物品数组（按类型）
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	TArray<URLInventoryItemInstance*> GetEquippedItemsByType(FGameplayTag SlotTypeTag);

	// 获取当前装备的物品（按类型）
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	URLInventoryItemInstance* GetEqeippedItemByType(FGameplayTag SlotTypeTag);
	//武器切换的输入回调函数
	UFUNCTION()
	void SwitchWeapon();
	
protected:
	virtual void BeginPlay() override;
	//回调函数，绑定父类的OnItemSlotUpdated
	UFUNCTION()
	virtual void OnEquipSlotUpdate(URLInventoryComponent* InventoryComponent,
		const FRLInventoryItemSlotHandle& SlotHandle,
		URLInventoryItemInstance* ItemInstance,
		URLInventoryItemInstance* PreviousItemInstance);

	virtual bool MakeItemEquipped_Internal(const FRLInventoryItemSlotHandle& SlotHandle,URLInventoryItemInstance* ItemInstance);
	virtual bool MakeItemUnequipped_Internal(const FRLInventoryItemSlotHandle& SlotHandle,URLInventoryItemInstance* ItemInstance);

	UFUNCTION()
	UAbilitySystemComponent* GetOwnerAbilitySystemComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TArray<FEquipmentSlotGroup> SlotGroups;

	UPROPERTY(VisibleAnywhere, Category = "Equipment")
	TArray<FRLInventoryItemInfoEntry> EquipmentInfos;

private:
	// 标识当前装备的武器
	UPROPERTY()
	FEquipWeapon CurrentWeapon;
};
