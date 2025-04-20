// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CloseCombatComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UCloseCombatComponent::UCloseCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
}


void UCloseCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	User = Cast<ACharacter>(GetOwner());

	if (User)
	{
		//生成武器
		CloseWeapon = GetWorld()->SpawnActorDeferred<ARL_BaseWeapon>(BP_CloseWeapon,FTransform(),User);
		CloseWeapon->WeaponOwner = User;
		CloseWeapon = Cast<ARL_BaseWeapon>(UGameplayStatics::FinishSpawningActor(CloseWeapon,FTransform()));

		//将武器绑到使用者的骨骼上
		EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
		EAttachmentRule RotationRule = EAttachmentRule::SnapToTarget;
		EAttachmentRule ScaleRule = EAttachmentRule::KeepRelative;
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(LocationRule,RotationRule,ScaleRule,true);
		CloseWeapon->AttachToComponent(User->GetMesh(),AttachmentTransformRules,FName("Socket_Weapon_Sword"));
	}
	
}

void UCloseCombatComponent::StartCombat(TSubclassOf<UGameplayEffect> DamageEffet) const
{
	CloseWeapon->StartCombat(DamageEffet);
}

void UCloseCombatComponent::EndCombat() const
{
	CloseWeapon->EndCombat();
}

