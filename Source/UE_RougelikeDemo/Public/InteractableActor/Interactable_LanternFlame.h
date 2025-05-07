// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor/Interactable_Base.h"
#include "Interactable_LanternFlame.generated.h"

class ARL_BasePlayerController;
class UGameplayAbility;
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

	// 交互回调,由输入处触发
	virtual void TryInteract() override;

	UPROPERTY(EditDefaultsOnly,Category="Datas")
	TSubclassOf<URL_UserWidget> WBP_SavePointClass;
	//存档点名字
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Datas")
	FString LanternFlameName;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Datas")
	TObjectPtr<URL_UserWidget> WBP_SavePoint;

	UFUNCTION(BlueprintImplementableEvent)
	void InitPointName();
	
private:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnComEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly,Category="Datas")
	TObjectPtr<UDataTable> SkillList;

	UPROPERTY()
	TObjectPtr<ARL_BasePlayerController> PlayerController;
};
