// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RL_PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URL_PlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class UAS_Player;
/**
 * 
 */
class UE_ROUGELIKEDEMO_API IRL_PlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 获取当前灵魂
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	int32 GetSoul() const;

	// 设置灵魂
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	void SetSoul(int32 NewSoul);

	// 获取当前等级
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	int32 GetLevel() const;

	// 增加等级
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	void AddLevel();

	// 获取技能点
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	int32 GetSpellPoints() const;

	// 增加技能点
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	void AddSpellPoints(int32 InSpellPoints);

	// 获取下一级所需的灵魂
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	int32 GetLevelRequirement(int32 InLevel) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	UAS_Player* GetPlayerAS() const;
};
