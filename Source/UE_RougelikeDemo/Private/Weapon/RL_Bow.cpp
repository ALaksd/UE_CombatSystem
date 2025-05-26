// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RL_Bow.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Weapon/RL_BaseWeapon.h"
#include "Weapon/RL_ProjectileBase.h"

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

void ARL_Bow::FireProjectile(FVector AimLocation)
{
	FVector SocketLocation = SkeletalMeshComponent->GetSocketLocation(SpawnSocke);
	FRotator SpawnRotation = (AimLocation - SocketLocation).Rotation();
	FTransform SpawnTransform = FTransform(SpawnRotation,SocketLocation);
	
	ARL_ProjectileBase* Arrow = GetWorld()->SpawnActor<ARL_ProjectileBase>(ArrowClass,SpawnTransform);
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
