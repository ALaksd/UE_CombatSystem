// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RL_BaseWeapon.h"

// Sets default values
ARL_BaseWeapon::ARL_BaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARL_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
//void ARL_BaseWeapon::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

