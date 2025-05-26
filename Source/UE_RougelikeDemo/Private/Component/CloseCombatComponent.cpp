// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CloseCombatComponent.h"
#include "UE_RougelikeDemo/InventorySystem/InventoryComponent/RLInventoryComponent_Equipment.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemInstance.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemDefinition.h"
#include "GameFramework/Character.h"
#include <UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_Attached.h>
#include "GameFramework/PlayerState.h"
#include <UE_RougelikeDemo/InventorySystem/RLItemFragment_EquipDynamicData.h>

#include "Weapon/RL_Sword.h"


UCloseCombatComponent::UCloseCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}


void UCloseCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	User = Cast<ACharacter>(GetOwner());

	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	//敌人类没有PlayerState,在Owner里面找到EquipmentInventoryComponent
	if (APlayerState* PlayerState = OwnerPawn->GetPlayerState())
	{
		EquipmentInventoryComponent = PlayerState->GetComponentByClass<URLInventoryComponent_Equipment>();

		check(EquipmentInventoryComponent);
		EquipmentInventoryComponent->OnEquipUpdate.AddDynamic(this, &UCloseCombatComponent::SwitchWeapon);
	}
	else
	{
		EquipmentInventoryComponent = GetOwner()->GetComponentByClass<URLInventoryComponent_Equipment>();
		check(EquipmentInventoryComponent);
		EquipmentInventoryComponent->OnEquipUpdate.AddDynamic(this, &UCloseCombatComponent::SwitchWeapon);
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

void UCloseCombatComponent::SwitchWeapon(URLInventoryItemInstance* NewWeapon)
{
	// 销毁之前的武器
	if (CurrentWeapon)
		CurrentWeapon->Destroy();

	if (!NewWeapon)
		return;

	// 创建新实例
	const URLItemFragment_Attached* Fragment = Cast<URLItemFragment_Attached>(
		NewWeapon->GetItemDefinition()->FindFragmentByClass(URLItemFragment_Attached::StaticClass()));

	const URLItemFragment_EquipDynamicData* EquipDynamicData = NewWeapon->FindFragmentByClass<URLItemFragment_EquipDynamicData>();

	if (Fragment && EquipDynamicData)
	{
		if (ARL_Sword* NewWeapon_T = Cast<ARL_Sword>(Fragment->AttachToActor(GetOwner(),NewWeapon)))
		{
			
			NewWeapon_T->SetActorHiddenInGame(false);
			NewWeapon_T->SetWeaponOwner(GetOwner());
			NewWeapon_T->SetWeaponLevel(EquipDynamicData->CurrentLevel);

			CurrentWeapon = NewWeapon_T;
		}
	}
	
}
