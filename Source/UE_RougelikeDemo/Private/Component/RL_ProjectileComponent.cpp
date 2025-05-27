

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

void URL_ProjectileComponent::PullBow(float Damage,FGameplayTag DamageTag)
{
	// 将弓弦与手部骨骼绑定
	if (Bow)
	{
		Bow->bIsPull=true;
		// 生成箭矢,绑定到弦上
		Bow->SpawnArrow(Damage,DamageTag);
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
	if (BowClass)
	{
		Bow = GetWorld()->SpawnActor<ARL_Bow>(BowClass);
		// 重置弓的相对位置和旋转
		Bow->GetRootComponent()->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
		if (Bow)
		{
			Bow->AttachToComponent(AttachCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		}
	}
	
}

