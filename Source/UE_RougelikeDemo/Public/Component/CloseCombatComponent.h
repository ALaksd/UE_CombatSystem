// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/RL_BaseWeapon.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryComponent.h"
#include "CloseCombatComponent.generated.h"

class ARL_Sword;
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
	FORCEINLINE ARL_Sword* GetCurrentWeapon() const { return CurrentWeapon; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE URLInventoryComponent_Equipment* GetEquipmentInventoryComponent() const { return EquipmentInventoryComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<ACharacter> User = nullptr;

	/** 主武器 */
	UPROPERTY()
	TObjectPtr<ARL_Sword> CurrentWeapon;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URLInventoryComponent_Equipment> EquipmentInventoryComponent;


public:	
	void StartCombat() const;
	void EndCombat() const;

private:
	UFUNCTION()
	void SwitchWeapon(URLInventoryItemInstance* NewWeapon);
};
