// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/RL_BaseWeapon.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "RL_WeaponCompoment.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API URL_WeaponCompoment : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URL_WeaponCompoment();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

//public:	
//	// Called every frame
//	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION()
	virtual void EquipWeapon();

	//生成的武器类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<ARL_BaseWeapon> WeaponClass;

	//插槽
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Weapon)
	FName SocketName;

	//附加到的角色
	ACharacter* AttachCharacter;

	//生成的武器的引用
	ARL_BaseWeapon* BaseWeapon;
		
};
