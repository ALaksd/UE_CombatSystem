// Copyright Epic Games, Inc. All Rights Reserved.

#include "UE_RougelikeDemoGameMode.h"
#include "UE_RougelikeDemoCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/RL_GameInstanceInterface.h"
#include "Character/RL_BaseCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "InventorySystem/RLInventorySubsystem.h"
#include <Engine/StreamableManager.h>
#include "Engine/AssetManager.h"

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

void AUE_RougelikeDemoGameMode::SpawnAllItemsFromDataTable()
{
	if (!ItemSpawnDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemSpawnDataTable is not set!"));
		return;
	}

	// 获取所有行
	TArray<FItemSpawnData*> AllRows;
	ItemSpawnDataTable->GetAllRows<FItemSpawnData>(TEXT("SpawnAllItems"), AllRows);

	// 收集所有需要加载的物品定义
	TArray<FSoftObjectPath> AssetsToLoad;
	for (FItemSpawnData* Row : AllRows)
	{
		if (Row && !Row->ItemDefinition.IsNull())
		{
			AssetsToLoad.AddUnique(Row->ItemDefinition.ToSoftObjectPath());
		}
	}

	if (AssetsToLoad.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid items to spawn in DataTable"));
		return;
	}

	// 异步加载所有资源
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(AssetsToLoad, [this, AllRows]()
		{
			// 加载完成后生成所有物品
			for (FItemSpawnData* Row : AllRows)
			{
				if (Row && Row->ItemDefinition.IsValid())
				{
					SpawnSingleItem(Row->ItemDefinition, Row->SpawnLocation);
				}
			}
		});
}


void AUE_RougelikeDemoGameMode::SpawnSingleItem(TSoftObjectPtr<URLInventoryItemDefinition> ItemDef, const FVector& Location)
{
	if (URLInventorySubsystem* InventorySubsystem = GetInventorySubsystem())
	{
		// 确保资源已加载
		if (ItemDef.IsPending())
		{
			ItemDef.LoadSynchronous();
		}

		if (ItemDef.IsValid())
		{
			InventorySubsystem->SpawnItemActorFromDefinition(ItemDef.Get(), Location);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load ItemDefinition: %s"), *ItemDef.ToString());
		}
	}
}

URLInventorySubsystem* AUE_RougelikeDemoGameMode::GetInventorySubsystem() const
{
	UGameInstance* GameInstance = GetGameInstance();
	return GameInstance ? GameInstance->GetSubsystem<URLInventorySubsystem>() : nullptr;
}
