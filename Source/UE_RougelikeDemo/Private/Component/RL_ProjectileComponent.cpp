

#include "Component/RL_ProjectileComponent.h"

#include "Weapon/RL_Bow.h"

URL_ProjectileComponent::URL_ProjectileComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void URL_ProjectileComponent::BeginPlay()
{
	Super::BeginPlay();
}

void URL_ProjectileComponent::PullBow()
{
	// 
}

void URL_ProjectileComponent::EquipWeapon()
{
	FActorSpawnParameters SpawnParameters;
	if (BowClass)
	{
		Bow = GetWorld()->SpawnActor<ARL_Bow>(BowClass, SpawnParameters);
		if (Bow)
		{
			Bow->AttachToComponent(AttachCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		}
	}
	
}

