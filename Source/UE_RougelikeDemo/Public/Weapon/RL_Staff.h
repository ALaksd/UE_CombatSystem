// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/RL_Bow.h"
#include "RL_Staff.generated.h"

class ARL_Projectile_Fireball;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API ARL_Staff : public ARL_Bow
{
	GENERATED_BODY()

public:
	ARL_Staff();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="Component")
	TObjectPtr<UStaticMeshComponent> Mesh;

	virtual void SpawnArrow(float Damage,FGameplayTag DamageTag,TArray<FFirebalLocation> Locations ,AActor* Target) override;

	virtual void FireProjectile() override;

private:
	TArray<ARL_Projectile_Fireball*> SpawnFireballs(float Damage,FGameplayTag DamageTag,TArray<FFirebalLocation> Locations ,AActor* Target);

	TArray<ARL_Projectile_Fireball*> Fireballs;


};

