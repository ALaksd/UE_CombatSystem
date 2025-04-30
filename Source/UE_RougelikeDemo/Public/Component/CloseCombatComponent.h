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

	UPROPERTY()
	TObjectPtr<ARL_BaseWeapon> CloseWeapon;

	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<ACharacter> User = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<ARL_BaseWeapon> BP_CloseWeapon;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URLInventoryComponent_Equipment> EquipmentInventoryComponent;


public:	
	void StartCombat() const;
	void EndCombat() const;
	UFUNCTION()
	void OnEquipSlotUpdate(URLInventoryItemInstance* ItemInstance,URLInventoryItemInstance* PreviousItemInstance);

	/*从背包装备/卸下/切换武器*/
	void EquipWeaponForInventory(URLInventoryItemInstance* ItemInstance);
	void UnEquipWeaponForInventory();
	void SwitchWeapon(URLInventoryItemInstance* ItemInstance);

	/*在手上装备/卸下/切换武器*/
	void EquipWeapon(ARL_BaseWeapon* NewWeapon);
	void UnEquipWeapon();
	void SwitchWeapon();
};
