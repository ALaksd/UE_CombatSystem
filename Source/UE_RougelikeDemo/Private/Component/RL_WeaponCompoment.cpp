// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/RL_WeaponCompoment.h"

// Sets default values for this component's properties
URL_WeaponCompoment::URL_WeaponCompoment()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void URL_WeaponCompoment::BeginPlay()
{
	Super::BeginPlay();

	AttachCharacter = Cast<ACharacter>(GetOwner());
	if (AttachCharacter)
	{
		EquipWeapon();
	}
	
}

void URL_WeaponCompoment::EquipWeapon()
{
	FActorSpawnParameters SpawnParameters;
	if (WeaponClass)
	{
		BaseWeapon = GetWorld()->SpawnActor<ARL_BaseWeapon>(WeaponClass, SpawnParameters);
		if (BaseWeapon)
		{
			BaseWeapon->AttachToComponent(AttachCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		}
	}

}


//// Called every frame
//void URL_WeaponCompoment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

