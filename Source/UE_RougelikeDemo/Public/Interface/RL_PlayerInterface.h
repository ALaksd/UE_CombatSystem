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

/**
 * 
 */
class UE_ROUGELIKEDEMO_API IRL_PlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// ��ȡ��ǰ���
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	int32 GetSoul() const;

	// �������
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	void SetSoul(int32 NewSoul);

	// ��ȡ��ǰ�ȼ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	int32 GetLevel() const;

	// ���ӵȼ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	void AddLevel();

	// ��ȡ���ܵ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	int32 GetSpellPoints() const;

	// ���Ӽ��ܵ�
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	void AddSpellPoints(int32 InSpellPoints);

	// ��ȡ��һ����������
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	int32 GetLevelRequirement(int32 InLevel) const;
};
