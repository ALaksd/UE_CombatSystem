// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InventorySystem/RLInventoryItemDefinition.h"
#include "RL_CharacterEunm.h"
#include "UE_RougelikeDemoGameMode.generated.h"

class URLInventorySubsystem;

USTRUCT(BlueprintType)
struct FItemSpawnData : public FTableRowBase
{
	GENERATED_BODY()

	// 物品定义资产引用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawning")
	TSoftObjectPtr<URLInventoryItemDefinition> ItemDefinition;

	// 生成位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawning")
	FVector SpawnLocation = FVector();
};

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

	// 生成数据表中所有物品
	UFUNCTION(BlueprintCallable, Category = "Item Spawning")
	void SpawnAllItemsFromDataTable();

	// 物品生成数据表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Spawning")
	UDataTable* ItemSpawnDataTable;


private:
	// 生成单个物品（内部使用）
	UFUNCTION()
	void SpawnSingleItem(TSoftObjectPtr<URLInventoryItemDefinition> ItemDef, const FVector& Location);

	// 获取库存子系统
	URLInventorySubsystem* GetInventorySubsystem() const;
};



