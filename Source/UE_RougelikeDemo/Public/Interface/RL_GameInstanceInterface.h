// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RL_CharacterEunm.h"
#include "RL_GameInstanceInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URL_GameInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_ROUGELIKEDEMO_API IRL_GameInstanceInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//��ȡѡ��Ľ�ɫ
	virtual ECharacterType GetSelectCharacter() = 0;
	//����ѡ��Ľ�ɫ
	virtual void SetSelectCharacter(ECharacterType CharacterType) = 0;
};
