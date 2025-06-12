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

	// 重写 GetDefaultPawnClassForController,带有BlueprintNativeEvent的函数需要用_Implementation 重写
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	//Tmap容器
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Character)
	TMap<ECharacterType, TSubclassOf<class ARL_BaseCharacter>> CharacterToClass;

};



