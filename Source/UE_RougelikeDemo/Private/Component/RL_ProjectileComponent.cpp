

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
	// 将弓弦与手部骨骼绑定
	if (Bow)
	{
		Bow->bIsPull=true;
		// 生成箭矢,绑定到弦上
		Bow->SpawnArrow();
	}
}

void URL_ProjectileComponent::FireProjectile()
{
	if (Bow)
	{
		Bow->FireProjectile();
	}
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

