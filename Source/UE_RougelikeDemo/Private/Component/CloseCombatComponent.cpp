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

	//if (User)
	//{
	//	//生成武器
	//	CloseWeapon = GetWorld()->SpawnActorDeferred<ARL_BaseWeapon>(BP_CloseWeapon,FTransform(),User);
	//	CloseWeapon->WeaponOwner = User;
	//	CloseWeapon = Cast<ARL_BaseWeapon>(UGameplayStatics::FinishSpawningActor(CloseWeapon,FTransform()));

	//	//将武器绑到使用者的骨骼上
	//	EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
	//	EAttachmentRule RotationRule = EAttachmentRule::SnapToTarget;
	//	EAttachmentRule ScaleRule = EAttachmentRule::KeepRelative;
	//	FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(LocationRule,RotationRule,ScaleRule,true);
	//	CloseWeapon->AttachToComponent(User->GetMesh(),AttachmentTransformRules,FName("Socket_Weapon_Sword"));
	//}

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
	CloseWeapon->StartCombat();
}

void UCloseCombatComponent::EndCombat() const
{
	CloseWeapon->EndCombat();
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
	const URLItemFragment_Attached* AttachedFragment =
		Cast<URLItemFragment_Attached>(ItemInstance->GetItemDefinition()->FindFragmentByClass(URLItemFragment_Attached::StaticClass())
		);
	if (AttachedFragment)
	{
		CloseWeapon = Cast<ARL_BaseWeapon>(AttachedFragment->AttachToActor(GetOwner()));
		CloseWeapon->WeaponOwner = User;
	}
}

void UCloseCombatComponent::UnEquipWeaponForInventory()
{
	if (CloseWeapon)
	{
		CloseWeapon->Destroy();
		CloseWeapon = nullptr;
	}
}


void UCloseCombatComponent::SwitchWeapon(URLInventoryItemInstance* ItemInstance)
{

}

void UCloseCombatComponent::EquipWeapon(ARL_BaseWeapon* NewWeapon)
{
}

void UCloseCombatComponent::UnEquipWeapon()
{
}

void UCloseCombatComponent::SwitchWeapon()
{
}

