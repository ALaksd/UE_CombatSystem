// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RL_CharacterEunm.h"
#include "UE_RougelikeDemoGameMode.generated.h"

UCLASS(minimalapi)
class AUE_RougelikeDemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUE_RougelikeDemoGameMode();

	// ��д GetDefaultPawnClassForController,����BlueprintNativeEvent�ĺ�����Ҫ��_Implementation ��д
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	//Tmap����
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Character)
	TMap<ECharacterType, TSubclassOf<class ARL_BaseCharacter>> CharacterToClass;

};



