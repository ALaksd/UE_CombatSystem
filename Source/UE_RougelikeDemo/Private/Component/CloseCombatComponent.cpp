// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CloseCombatComponent.h"
#include "UE_RougelikeDemo/InventorySystem/InventoryComponent/RLInventoryComponent_Equipment.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemInstance.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemDefinition.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include <UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_Attached.h>
#include "GameFramework/PlayerState.h"


UCloseCombatComponent::UCloseCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}


void UCloseCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	User = Cast<ACharacter>(GetOwner());

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	APlayerState* PlayerState = OwnerPawn->GetPlayerState();

	//敌人类没有PlayerState,在Owner里面找到EquipmentInventoryComponent
	if (PlayerState)
	{
		EquipmentInventoryComponent = PlayerState->GetComponentByClass<URLInventoryComponent_Equipment>();

		check(EquipmentInventoryComponent);
		EquipmentInventoryComponent->OnEquipUpdate.AddDynamic(this, &UCloseCombatComponent::OnEquipSlotUpdate);
	}
	else
	{
		EquipmentInventoryComponent = GetOwner()->GetComponentByClass<URLInventoryComponent_Equipment>();
		check(EquipmentInventoryComponent);
		EquipmentInventoryComponent->OnEquipUpdate.AddDynamic(this, &UCloseCombatComponent::OnEquipSlotUpdate);
	}

}

void UCloseCombatComponent::StartCombat() const
{
	CurrentWeapon->StartCombat();
}

void UCloseCombatComponent::EndCombat() const
{
	CurrentWeapon->EndCombat();
}

void UCloseCombatComponent::OnEquipSlotUpdate(URLInventoryItemInstance* ItemInstance, URLInventoryItemInstance* PreviousItemInstance)
{
	if (PreviousItemInstance)
	{
		UnEquipWeaponForInventory();
	}
	if (ItemInstance)
	{
		EquipWeaponForInventory(ItemInstance);
	}
}

void UCloseCombatComponent::EquipWeaponForInventory(URLInventoryItemInstance* ItemInstance)
{
	if (ARL_BaseWeapon* Weapon = GetWeaponFromInstance(ItemInstance))
	{
		Weapon->SetActorHiddenInGame(false);
		CurrentWeapon = Weapon;
	}
}

void UCloseCombatComponent::UnEquipWeaponForInventory()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
		CurrentWeapon = nullptr;
	}
}


void UCloseCombatComponent::SwitchWeapon()
{
	 // 获取有效武器列表
    TArray<ARL_BaseWeapon*> ValidWeapons;
    for (ARL_BaseWeapon* Weapon : WeaponPool)
    {
        if (Weapon && !Weapon->IsHidden())
        {
            ValidWeapons.Add(Weapon);
        }
    }
    
    if (ValidWeapons.Num() < 2) return;

    // 计算新索引
    const int32 NewIndex = (CurrentWeaponIndex + 1) % ValidWeapons.Num();
    
    // 隐藏当前武器
    if (ARL_BaseWeapon* Current = ValidWeapons[CurrentWeaponIndex])
    {
        Current->SetActorHiddenInGame(true);
    }

    // 显示新武器
    ARL_BaseWeapon* NewWeapon = ValidWeapons[NewIndex];
    NewWeapon->SetActorHiddenInGame(false);
	CurrentWeapon = NewWeapon;
    CurrentWeaponIndex = NewIndex;
}

//没用到
void UCloseCombatComponent::PreloadWeapons()
{
	TArray<FRLInventoryItemSlotHandle> AllSlots = EquipmentInventoryComponent->GetSlotsByType(
		FGameplayTag::RequestGameplayTag("Equipment.Type.Weapon"));

	for (const auto& Slot : AllSlots)
	{
		if (URLInventoryItemInstance* Instance = EquipmentInventoryComponent->GetItemInstanceInSlot(Slot))
		{
			if (ARL_BaseWeapon* Weapon = GetWeaponFromInstance(Instance))
			{
				Weapon->SetActorHiddenInGame(true);
				WeaponPool.Add(Weapon);
			}
		}
	}
}

ARL_BaseWeapon* UCloseCombatComponent::GetWeaponFromInstance(URLInventoryItemInstance* Instance)
{
	for (AActor* WeaponActor : WeaponPool)
	{
		if (IRL_ItemInstanceHolder::Execute_GetItemInstance(WeaponActor) == Instance)
		{
			return Cast<ARL_BaseWeapon>(WeaponActor);
		}
	}

	// 创建新实例
	const URLItemFragment_Attached* Fragment = Cast<URLItemFragment_Attached>(
		Instance->GetItemDefinition()->FindFragmentByClass(URLItemFragment_Attached::StaticClass()));

	if (Fragment)
	{
		ARL_BaseWeapon* NewWeapon = Cast<ARL_BaseWeapon>(Fragment->AttachToActor(GetOwner(),Instance));
		NewWeapon->SetActorHiddenInGame(true);
		NewWeapon->SetWeaponOwner(GetOwner());
		WeaponPool.Add(NewWeapon);
		return NewWeapon;
	}
	return nullptr;
}

