// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorComponent.generated.h"


class URLInventoryItemInstance;
class URLInventoryComponent_Equipment;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API UArmorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UArmorComponent();

	TObjectPtr<AActor> CurrentArmor;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	TObjectPtr<ACharacter> Owner;

	TObjectPtr<URLInventoryComponent_Equipment> EquipmentInventoryComponent;

	UFUNCTION()
	void SwitchArmor(URLInventoryItemInstance* NewWeapon);
};
