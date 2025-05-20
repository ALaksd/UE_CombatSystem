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

	//ʵ�ֽӿں���
	UFUNCTION(BlueprintCallable)
	ECharacterType GetSelectCharacter() override;

	UFUNCTION(BlueprintCallable)
	void SetSelectCharacter(ECharacterType CharacterType) override;

	//����Ľ�ɫ
	ECharacterType SelectCharacter;
};
