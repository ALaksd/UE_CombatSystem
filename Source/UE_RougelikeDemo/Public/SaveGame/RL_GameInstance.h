// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/RL_GameInstanceInterface.h"
#include "RL_CharacterEunm.h"
#include "RL_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_GameInstance : public UGameInstance,public IRL_GameInstanceInterface
{
	GENERATED_BODY()
	
public:

	//实现接口函数
	UFUNCTION(BlueprintCallable)
	ECharacterType GetSelectCharacter() override;

	UFUNCTION(BlueprintCallable)
	void SetSelectCharacter(ECharacterType CharacterType) override;

	//保存的角色
	ECharacterType SelectCharacter;
};
