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

void ARL_Bow::SpawnArrow(float Damage,FGameplayTag DamageTag,AActor* Target)
 {
	FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(SpawnSocke);
	FRotator SocketRotation = SkeletalMeshComponent->GetSocketRotation(SpawnSocke);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s"), *SocketRotation.ToString()));
	
	SocketRotation.Yaw += 90;
	FTransform SpawnTransform =FTransform(); //FTransform(SocketRotation,SocketLocation,FVector(1,1,1));

	// 生成箭矢
	Arrow = GetWorld()->SpawnActor<ARL_ProjectileBase>(ArrowClass,SpawnTransform);
	Arrow->TargetActor=Target;
	Arrow->SetWeaponOwner(WeaponOwner);

	//Arrow->SetActorRotation(SocketRotation);
	
	// 将箭矢绑到弓弦上
	EAttachmentRule LocationRules = EAttachmentRule::KeepRelative;
	EAttachmentRule RotationRules = EAttachmentRule::KeepWorld;
	EAttachmentRule ScaleRules = EAttachmentRule::KeepWorld;
	FAttachmentTransformRules Rules = FAttachmentTransformRules(LocationRules,RotationRules,ScaleRules,false);
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
