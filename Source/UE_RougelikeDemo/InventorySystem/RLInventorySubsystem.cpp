// Fill out your copyright notice in the Description page of Project Settings.


#include "RLInventorySubsystem.h"
#include "RLInventoryItemInstance.h"
#include "RLInventoryItemDefinition.h"
#include "Fragments/RLItemFragment_PhysicalDisplay.h"


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

	return NewInstance;
}

AActor* URLInventorySubsystem::SpawnItemActorFromInstance(URLInventoryItemInstance* ItemInstance, const FVector& Location)
{
	if (!ItemInstance || !ItemInstance->GetItemDefinition())
		return nullptr;

	const URLItemFragment_PhysicalDisplay* Fragment =
		Cast<URLItemFragment_PhysicalDisplay>(
			ItemInstance->GetItemDefinition()->FindFragmentByClass(URLItemFragment_PhysicalDisplay::StaticClass())
		);

	if (!Fragment || !Fragment->ItemActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No valid ItemActorClass in Fragment."));
		return nullptr;
	}

	UWorld* World = GetWorld();
	if (!World) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* SpawnedActor = World->SpawnActor<AActor>(Fragment->ItemActorClass, Location, FRotator::ZeroRotator, SpawnParams);
	return SpawnedActor;
}

AActor* URLInventorySubsystem::SpawnItemActorFromDefinition(URLInventoryItemDefinition* ItemDefinition, const FVector& Location)
{
	URLInventoryItemInstance* NewInstance = GenerateItemInstance(ItemDefinition);
	AActor* SpawnedActor = SpawnItemActorFromInstance(NewInstance,Location);
	return SpawnedActor;
}
