// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile/RL_ProjectileBase.h"
#include "RL_ProjectileArrow.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API ARL_ProjectileArrow : public ARL_ProjectileBase
{
	GENERATED_BODY()
	
protected:
	//抛射物击中后产生的物理效果
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, const FHitResult& HitResult) override;

	UFUNCTION(BlueprintImplementableEvent)
	void StopNiagara();
};
