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
	CapsuleComponent=CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	USceneComponent* ParentRoot = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("RootComponent")));
	if (ParentRoot)
		ParentRoot->DestroyComponent(); // 销毁父类根组件

	
	SkeletalMeshComponent=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

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

void ARL_Bow::SpawnArrow(float Damage,FGameplayTag DamageTag)
{
	FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(SpawnSocke);
	FRotator SocketRotation = SkeletalMeshComponent->GetSocketRotation(SpawnSocke);
	FTransform SpawnTransform = FTransform(SocketRotation,SocketLocation);

	// 生成箭矢
	Arrow = GetWorld()->SpawnActor<ARL_ProjectileBase>(ArrowClass,SpawnTransform);

	// 将箭矢绑到弓弦上
	FAttachmentTransformRules Rules = FAttachmentTransformRules::SnapToTargetIncludingScale;
	Arrow->AttachToComponent(GetMesh(),Rules,SpawnSocke);
	
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
	if (BowDrawSound && GetWorld())
	{
		SoundToPlay = UGameplayStatics::SpawnSoundAtLocation(this,BowDrawSound, GetActorLocation());
	}
}

void ARL_Bow::PullBowEnd()
{
	BowState = EBowState::Idle;
	if (SoundToPlay)
	{
		SoundToPlay->Stop();
	}
}
