// Fill out your copyright notice in the Description page of Project Settings.


#include "RLInventorySubsystem.h"
#include "RLInventoryItemInstance.h"
#include "RLInventoryItemDefinition.h"
#include "Fragments/RLItemFragment_Pickup.h"
#include "Item/Item_Pickup.h"
#include "RLItemFragment_EquipDynamicData.h"
#include "Fragments/RLItemFragment_WeaponLevelData.h"
#include "RLInventoryComponent.h"
#include <Player/RL_PlayerState.h>


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

bool URLInventorySubsystem::UpgradeWeapon(URLInventoryItemInstance* WeaponInstance, URLInventoryComponent* OwnerInventory)
{
	// 参数有效性检查
	if (!WeaponInstance || !OwnerInventory)
	{
		return false;
	}

	// 获取等级Fragment
	const URLItemFragment_WeaponLevelData* LevelFragment = WeaponInstance->FindFragmentByClass<URLItemFragment_WeaponLevelData>();
	const URLItemFragment_EquipDynamicData* DynamicFragment = WeaponInstance->FindFragmentByClass<URLItemFragment_EquipDynamicData>();
	if (LevelFragment && DynamicFragment)
	{
		//获取相关数据
		int32 CurrentLevel = DynamicFragment->CurrentLevel;
		FWeaponLevelData WeaponLevelData = LevelFragment->GetWeaponLevelData(CurrentLevel);
		int32 NeededFragment = WeaponLevelData.NeededFragment;
		int32 NeededSoul = WeaponLevelData.NeededCurrency;
		FGameplayTag NeededTag = WeaponLevelData.FragmentTag;


		FRLInventoryItemSlotHandle SlotHandle = OwnerInventory->GetSlotHandleByTag(NeededTag);
		URLInventoryItemInstance* SlotInstance = OwnerInventory->GetItemInstanceInSlot(SlotHandle);
		if (SlotHandle.SlotId == -1 || !SlotInstance)
			return false;

		//背包有的武器强化碎片数量
		int32 CurrentFragmentNum = SlotInstance->GetCurrentStack();

		//获取所持有的灵魂数量
		ARL_PlayerState* RL_PlayerState = Cast<ARL_PlayerState>(OwnerInventory->GetOwner());
		int32 CurrentSoul = RL_PlayerState->GetSoul();

		// 检查材料是否足够
		if (CurrentFragmentNum >= NeededFragment && CurrentSoul >= NeededSoul)
		{
			// 扣除材料
			OwnerInventory->RemoveItemFromInventory(SlotHandle, NeededFragment);
			RL_PlayerState->SetSoul(CurrentSoul - NeededSoul);

			//执行升级
			DynamicFragment->AddLevel();

			return true;
		}
		return false;
	}
	return false;
}

FWeaponLevelData URLInventorySubsystem::GetCurrentWeaponLevelData(URLInventoryItemInstance* WeaponInstance)
{
	// 获取等级Fragment
	const URLItemFragment_WeaponLevelData* LevelFragment = WeaponInstance->FindFragmentByClass<URLItemFragment_WeaponLevelData>();
	const URLItemFragment_EquipDynamicData* DynamicFragment = WeaponInstance->FindFragmentByClass<URLItemFragment_EquipDynamicData>();
	if (LevelFragment && DynamicFragment)
	{
		//获取相关数据
		int32 CurrentLevel = DynamicFragment->CurrentLevel;
		FWeaponLevelData WeaponLevelData = LevelFragment->GetWeaponLevelData(CurrentLevel);
		return WeaponLevelData;
	}
	return FWeaponLevelData();
}

int32 URLInventorySubsystem::GetCurrentWeaponFragmentNum(URLInventoryComponent* OwnerInventory, FGameplayTag ItemTag)
{
	FRLInventoryItemSlotHandle SlotHandle = OwnerInventory->GetSlotHandleByTag(ItemTag);
	URLInventoryItemInstance* SlotInstance = OwnerInventory->GetItemInstanceInSlot(SlotHandle);
	if (SlotHandle.SlotId == -1 || !SlotInstance)
		return 0;

	//背包有的武器强化碎片数量
	int32 CurrentFragmentNum = SlotInstance->GetCurrentStack();
	return CurrentFragmentNum;
}

int32 URLInventorySubsystem::GetCurrentSoul(URLInventoryComponent* OwnerInventory)
{
	ARL_PlayerState* RL_PlayerState = Cast<ARL_PlayerState>(OwnerInventory->GetOwner());
	int32 CurrentSoul = RL_PlayerState->GetSoul();
	return CurrentSoul;
}

