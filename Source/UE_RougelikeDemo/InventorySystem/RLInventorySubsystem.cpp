// Fill out your copyright notice in the Description page of Project Settings.


#include "RLInventorySubsystem.h"
#include "RLInventoryItemInstance.h"
#include "RLInventoryItemDefinition.h"
#include "Fragments/RLItemFragment_Pickup.h"
#include "Item/Item_Pickup.h"
#include "RLItemFragment_EquipDynamicData.h"


URLInventoryItemInstance* URLInventorySubsystem::GenerateItemInstance(URLInventoryItemDefinition* ItemDefinition)
{
	// 有效性验证
	if (!IsValid(ItemDefinition))
	{
		UE_LOG(LogTemp, Error, TEXT("GenerateItemInstance failed: Invalid ItemDefinition"));
		return nullptr;
	}

	// 创建实例
	URLInventoryItemInstance* NewInstance = NewObject<URLInventoryItemInstance>(this);
	NewInstance->SetItemDefinition(ItemDefinition);

	//添加动态实例，这里所有都添加

	URLItemFragment_EquipDynamicData* NewDynamicFragment = NewObject<URLItemFragment_EquipDynamicData>();
	NewInstance->AddDynamicFragments(NewDynamicFragment);
	NewDynamicFragment->Owner = NewInstance;
	return NewInstance;
}

AItem_Pickup* URLInventorySubsystem::SpawnItemActorFromInstance(URLInventoryItemInstance* ItemInstance, const FVector& Location)
{
	if (!ItemInstance || !ItemInstance->GetItemDefinition())
		return nullptr;

	const URLItemFragment_Pickup* Fragment =
		Cast<URLItemFragment_Pickup>(
			ItemInstance->GetItemDefinition()->FindFragmentByClass(URLItemFragment_Pickup::StaticClass())
		);
	
	if (!Fragment || !Fragment->ItemActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid ItemActorClass in Fragment."));
		return nullptr;
	}

	Fragment->OnInstancedCreate(ItemInstance);

	UWorld* World = GetWorld();
	if (!World) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AItem_Pickup* SpawnedActor = World->SpawnActor<AItem_Pickup>(Fragment->ItemActorClass, Location, FRotator::ZeroRotator, SpawnParams);
	SpawnedActor->ItemInstance = ItemInstance;
	return SpawnedActor;
}

AActor* URLInventorySubsystem::SpawnItemActorFromDefinition(URLInventoryItemDefinition* ItemDefinition, const FVector& Location)
{
	URLInventoryItemInstance* NewInstance = GenerateItemInstance(ItemDefinition);
	AActor* SpawnedActor = SpawnItemActorFromInstance(NewInstance,Location);
	return SpawnedActor;
}
