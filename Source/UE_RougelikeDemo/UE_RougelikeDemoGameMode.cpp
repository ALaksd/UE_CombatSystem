// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_RougelikeDemoGameMode.h"
#include "UE_RougelikeDemoCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/RL_GameInstanceInterface.h"
#include "Character/RL_BaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUE_RougelikeDemoGameMode::AUE_RougelikeDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}

UClass* AUE_RougelikeDemoGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (IRL_GameInstanceInterface* GameInstanceInterface = Cast<IRL_GameInstanceInterface>(GameInstance))
	{
		ECharacterType SelectedCharacter = GameInstanceInterface->GetSelectCharacter();

		// 从 TMap 中找到对应的角色类
		if (CharacterToClass.Contains(SelectedCharacter))
		{
			return *CharacterToClass.Find(SelectedCharacter);
		}
	}

	// 如果没有找到对应角色，调用父类实现
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}
