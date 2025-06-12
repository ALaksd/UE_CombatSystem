// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RL_Staff.h"

#include "GameplayTagContainer.h"
#include "Weapon/Projectile/RL_Projectile_Fireball.h"
#include "Components/StaticMeshComponent.h"
#include "Data/Structs.h"

ARL_Staff::ARL_Staff()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshCom");
	Mesh->SetupAttachment(RootComponent);
}

void ARL_Staff::SpawnArrow(float Damage,FGameplayTag DamageTag,TArray<FFirebalLocation> Locations ,AActor* Target)
{
	if (Locations.Num() <= 0)
		return ;

	SpawnFireballs(Damage,DamageTag,Locations,Target);
}

void ARL_Staff::FireProjectile()
{
	for (int32 i = Fireballs.Num() - 1; i >= 0; --i)
	{
		if (ARL_Projectile_Fireball* Fireball = Fireballs[i])
		{
			Fireball->FireProjectile();
			Fireballs.RemoveAt(i);
		}
	}
}

void ARL_Staff::FireProjectile(bool bIsOneFire)
{
	if (bIsOneFire)
		if (Fireballs.Num()>0)
		{
			Fireballs[0]->FireProjectile();
			Fireballs.RemoveAt(0);
		}
}

TArray<ARL_Projectile_Fireball*> ARL_Staff::SpawnFireballs(float Damage,FGameplayTag DamageTag,TArray<FFirebalLocation> Locations ,AActor* Target)
{
	if (Locations.Num() <= 0) return Fireballs;

	for (FFirebalLocation& LocationOffset : Locations)
	{
		// 解析位置偏移
		FVector RightOffset = WeaponOwner->GetActorRightVector()*LocationOffset.Location.X;
		FVector ForwardOffset = WeaponOwner->GetActorForwardVector()*LocationOffset.Location.Y;
		FVector UpOffset = WeaponOwner->GetActorUpVector()*LocationOffset.Location.Z;
		FVector LocationOff = RightOffset+ForwardOffset+UpOffset;

		// 计算新的生成位置
		FVector SpawnLocation = WeaponOwner->GetActorLocation() + LocationOff;
		FRotator SpawnRotation = WeaponOwner->GetActorRotation() + LocationOffset.Rotation;
		// 生成火球
		FActorSpawnParameters SpawnParams;
		ARL_Projectile_Fireball* Fireball = GetWorld()->SpawnActor<ARL_Projectile_Fireball>(ArrowClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (Fireball)
		{
			Fireball->Target=Target;
			Fireball->InitProjectile(Damage,DamageTag);
			Fireball->SetWeaponOwner(WeaponOwner);
			Fireballs.Add(Fireball);

			FAttachmentTransformRules AttachRules= FAttachmentTransformRules(
				EAttachmentRule::KeepWorld,
				EAttachmentRule::SnapToTarget, 
				EAttachmentRule::KeepWorld, 
				false);
			Fireball->AttachToActor(WeaponOwner,AttachRules);
			Fireball->SetActorLocation(SpawnLocation);
		}
	}
	
	return Fireballs;
}


