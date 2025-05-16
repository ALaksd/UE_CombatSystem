// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RL_SavePointSubsystem.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FSavePointData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName PointID;

	UPROPERTY(BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite)
	bool bActive;

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite)
	FName MapName;
};
 
UCLASS()
class UE_ROUGELIKEDEMO_API URL_SavePointSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// 注册存档点
	UFUNCTION(BlueprintCallable)
	void RegisterSavePoint(const FSavePointData& SavePointData);

	// 获取所有可用存档点
	UFUNCTION(BlueprintCallable)
	TArray<FSavePointData> GetAvailableSavePoints() const;

	// 传送到指定存档点
	UFUNCTION(BlueprintCallable)
	void TravelToSavePoint(const FName& TargetPointID);

	//传送到当前存档点，也就是最后一次交互的存档点
	UFUNCTION(BlueprintCallable)
	void TravelToCurrentPoint();

	UFUNCTION()
	FORCEINLINE void SetCurrentSavaPoint(FName PointID) {CurrentSavePointID = PointID;}

private:
	UPROPERTY()
	TMap<FName, FSavePointData> SavePoints;

	void HandlePostLoadMap(UWorld* LoadedWorld);

	FName PendingTravelPointID;

	FName CurrentSavePointID;
	
};
