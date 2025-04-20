// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/RL_BaseWeapon.h"
#include "CloseCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API UCloseCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCloseCombatComponent();


private:
	UPROPERTY()
	TObjectPtr<ACharacter> User = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARL_BaseWeapon> BP_CloseWeapon;

	UPROPERTY()
	TObjectPtr<ARL_BaseWeapon> CloseWeapon;
	
protected:
	virtual void BeginPlay() override;

public:	
	void StartCombat(TSubclassOf<UGameplayEffect> DamageEffet) const;
	void EndCombat() const;
		
};
