// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemInstance.h"
#include "Item_Pickup.generated.h"

class UNiagaraComponent;
class USphereComponent;

UCLASS()
class UE_ROUGELIKEDEMO_API AItem_Pickup : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem_Pickup();

	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<USphereComponent> SphereCom;

	//背包需要的数据
	URLInventoryItemInstance* ItemInstance;
	
	void SetIdldEffect(UParticleSystem* InEffect) { IdleEffect = InEffect;}
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UParticleSystem* IdleEffect;

	UPROPERTY()
	UParticleSystemComponent* IdleEffectComponent;
private:
	UFUNCTION()
	void OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
