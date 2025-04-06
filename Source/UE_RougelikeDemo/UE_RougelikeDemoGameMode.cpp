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

		// �� TMap ���ҵ���Ӧ�Ľ�ɫ��
		if (CharacterToClass.Contains(SelectedCharacter))
		{
			return *CharacterToClass.Find(SelectedCharacter);
		}
	}

	// ���û���ҵ���Ӧ��ɫ�����ø���ʵ��
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}
