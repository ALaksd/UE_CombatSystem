// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RL_BaseWeapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Interface/RL_DamageInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UE_RougelikeDemo/InventorySystem/RLItemFragment_EquipDynamicData.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_WeaponLevelData.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemInstance.h"
#include "UE_RougelikeDemo/UE_RougelikeDemo.h"
#include <GAS/RL_AbilitySystemLibrary.h>
#include <Interface/RL_EnemyInterface.h>
#include <Interface/RL_PlayerInterface.h>
#include "GAS/AS/AS_Player.h"
#include <System/RL_SanitySubsystem.h>

// Sets default values
ARL_BaseWeapon::ARL_BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	
	WeaponASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	WeaponAttribute = CreateDefaultSubobject<UAS_Weapon>(TEXT("AttributeSet"));
}


void ARL_BaseWeapon::SetWeaponLevel(int32 NewLevel)
{
	//获取武器等级数据的Fragment
	if (!ItemInstance) return;

	const URLItemFragment_WeaponLevelData* WeaponLevelDataFrag = ItemInstance->FindFragmentByClass<URLItemFragment_WeaponLevelData>();
	if (!WeaponLevelDataFrag) return;

	if (NewLevel > WeaponLevelDataFrag->MaxLevel)
	{
		return;
	}
	WeaponLevel = NewLevel;


	if (WeaponAttribute && WeaponASC)
	{
		// 创建临时GE修改属性
		FGameplayEffectContextHandle Context = WeaponASC->MakeEffectContext();
		Context.AddSourceObject(this);

		UGameplayEffect* GE = NewObject<UGameplayEffect>();
		GE->DurationPolicy = EGameplayEffectDurationType::Instant;

		//仅设置Damage
		FGameplayModifierInfo DamageMod;
		DamageMod.Attribute = UAS_Weapon::GetDamageAttribute();
		DamageMod.ModifierOp = EGameplayModOp::Override;
		DamageMod.ModifierMagnitude = FScalableFloat(WeaponLevelDataFrag->GetWeaponLevelData(WeaponLevel).BaseDamage);
		GE->Modifiers.Add(DamageMod);

		WeaponASC->ApplyGameplayEffectToSelf(GE, 1.0f, Context);
	}
}
