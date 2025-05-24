// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RL_WeaponCompoment.h"
#include "RL_ProjectileComponent.generated.h"


class ARL_Bow;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API URL_ProjectileComponent : public URL_WeaponCompoment
{
	GENERATED_BODY()

public:	
	URL_ProjectileComponent();

protected:
	virtual void BeginPlay() override;

	// 拉弓
	void PullBow();

	UFUNCTION()
	virtual void EquipWeapon() override;

	// 要生成的弓的class
	UPROPERTY(EditDefaultsOnly,Category="Bow")
	TSubclassOf<ARL_Bow> BowClass;


private:
	// 生成的弓的引用
	UPROPERTY()
	TObjectPtr<ARL_Bow> Bow;


	
};
