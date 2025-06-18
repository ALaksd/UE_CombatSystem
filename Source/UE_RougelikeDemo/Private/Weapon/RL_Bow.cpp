// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RL_Bow.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Weapon/RL_BaseWeapon.h"
#include "Weapon/Projectile/RL_ProjectileBase.h"
#include "Data/Structs.h"



ARL_Bow::ARL_Bow()
{
	// CapsuleComponent=CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	// SetRootComponent(CapsuleComponent);
	//
	// USceneComponent* ParentRoot = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("RootComponent")));
	// if (ParentRoot)
	// 	ParentRoot->DestroyComponent(); // 销毁父类根组件

	
	SkeletalMeshComponent=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	WeaponType = E_WeaponType::Bow;
	
}

void ARL_Bow::FireProjectile()
{
	if (Arrow)
	{
		Arrow->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Arrow->FireProjectile();
	}
}

void ARL_Bow::FireProjectile(bool bIsOneFire)
{
}

void ARL_Bow::SpawnArrow(float Damage, FGameplayTag DamageTag, AActor* Target)
{
	// 获取插槽的位置信息
	FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(SpawnSocke);

	// 获取插槽的旋转信息
	FRotator SocketRotation;

	// 如果有目标，计算箭矢的方向
	FVector Direction = FVector::ZeroVector;
	if (Target)
	{
		Direction = (Target->GetActorLocation() - SocketLocation).GetSafeNormal();
		// 计算箭矢应朝向目标的旋转
		SocketRotation = Direction.Rotation();
	}

	// 设置箭矢的生成位置和旋转
	FTransform SpawnTransform(SocketRotation, SocketLocation);

	// 生成箭矢
	Arrow = GetWorld()->SpawnActor<ARL_ProjectileBase>(ArrowClass, SpawnTransform);
	if (Arrow)
	{
		Arrow->TargetActor = Target;
		Arrow->SetWeaponOwner(WeaponOwner);
		Arrow->InitProjectile(Damage, DamageTag);

		// 将箭矢绑到弓弦上
		EAttachmentRule LocationRules = EAttachmentRule::KeepWorld;
		EAttachmentRule RotationRules = EAttachmentRule::KeepRelative;
		EAttachmentRule ScaleRules = EAttachmentRule::KeepWorld;
		FAttachmentTransformRules Rules = FAttachmentTransformRules(LocationRules, RotationRules, ScaleRules, false);
		Arrow->AttachToComponent(GetMesh(), Rules, SpawnSocke);
	}
}

void ARL_Bow::SpawnArrow(float Damage,FGameplayTag DamageTag,TArray<FFirebalLocation> Locations ,AActor* Target)
{
}

void ARL_Bow::FireProjectile(FVector AimLocation)
{
	FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(SpawnSocke);
	FRotator SpawnRotation = (AimLocation - SocketLocation).Rotation();
	FTransform SpawnTransform = FTransform(SpawnRotation,SocketLocation);
	
	Arrow = GetWorld()->SpawnActor<ARL_ProjectileBase>(ArrowClass,SpawnTransform);
}

void ARL_Bow::PullBowStart()
{
	BowState = EBowState::Draw;
}

void ARL_Bow::PullBowEnd()
{
	BowState = EBowState::Idle;
}
