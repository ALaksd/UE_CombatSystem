// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor/Interactable_Base.h"
#include "Interactable_LanternFlame.generated.h"

class URL_UserWidget;
class USphereComponent;
/**
 * 存档点
 */
UCLASS()
class UE_ROUGELIKEDEMO_API AInteractable_LanternFlame : public AInteractable_Base
{
	GENERATED_BODY()

public:
	AInteractable_LanternFlame();

	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<USphereComponent> SphereCom;
	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshCom;

	virtual void TryInteract() override;

	UPROPERTY(EditDefaultsOnly,Category="Datas")
	TSubclassOf<UUserWidget> WBP_SavePointClass;
	
private:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnComEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	TObjectPtr<URL_UserWidget> WBP_SavePoint;
};
