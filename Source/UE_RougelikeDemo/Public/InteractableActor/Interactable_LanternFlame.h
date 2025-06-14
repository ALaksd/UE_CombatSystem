// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor/Interactable_Base.h"
#include "UE_RougelikeDemo/Interact/InteractableActor.h"
#include "Interactable_LanternFlame.generated.h"

class ARL_BasePlayerController;
class UGameplayAbility;
class URL_UserWidget;
class USphereComponent;
class UBoxComponent;
class ARL_EnemySpawnPoint;
/**
 * 存档点
 */

//DECLARE_DELEGATE_OneParam()
UCLASS()
class UE_ROUGELIKEDEMO_API AInteractable_LanternFlame : public AInteractableActor
{
	GENERATED_BODY()

public:
	AInteractable_LanternFlame();
	UPROPERTY(EditDefaultsOnly,Category="LanternFlame|Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshCom;

	UPROPERTY(EditDefaultsOnly,Category="LanternFlame|Components")
	TObjectPtr<USkeletalMeshComponent> SKM1;

	UPROPERTY(EditDefaultsOnly,Category="LanternFlame|Components")
	TObjectPtr<USkeletalMeshComponent> SKM2;

	UPROPERTY(EditDefaultsOnly,Category="LanternFlame|Components")
	TObjectPtr<USkeletalMeshComponent> SKM3;

	UPROPERTY(EditDefaultsOnly,Category="LanternFlame|Components")
	TObjectPtr<USkeletalMeshComponent> SKM4;

	UPROPERTY(EditDefaultsOnly,Category="LanternFlame|Components")
	TObjectPtr<USkeletalMeshComponent> DIZUO;

	// 交互回调,由输入处触发
	//virtual void TryInteract() override;

	UPROPERTY(EditDefaultsOnly,Category="LanternFlame|Datas")
	TSubclassOf<URL_UserWidget> WBP_SavePointClass;
	//存档点名字
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="LanternFlame|Datas")
	FString LanternFlameName;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="LanternFlame|Datas")
	TObjectPtr<URL_UserWidget> WBP_SavePoint;

	UFUNCTION(BlueprintImplementableEvent)
	void InitPointName();

	void ResetEnemySpawn();

	UPROPERTY(EditDefaultsOnly,Category="LanternFlame|Datas")
	TObjectPtr<UDataTable> SkillList;

	UPROPERTY()
	TObjectPtr<ARL_BasePlayerController> PlayerController;

	//是否激活
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(EditAnywhere,Category = "LanternFlame|Enemy")
	TArray<ARL_EnemySpawnPoint*> SpawnPoints;

	UPROPERTY()
	TMap<USkeletalMeshComponent*, UAnimSequence*> SkeletalToAnimMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimSequence*> Anims;
	
	void ActivatePoint();
protected:
	//蓝图实现事件，在ActivatePoint里调用，用于设置激活特效等以及弹出UI等
	UFUNCTION(BlueprintImplementableEvent)
	void OnPointActivaete();

	/** 敌人生成 */
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlapedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

	//是否已生成敌人，传送后应该重置
	UPROPERTY(BlueprintReadWrite)
	bool bSpawned = false;

private:
	virtual void BeginPlay() override;
	
};
