

#include "Component/RL_ProjectileComponent.h"

#include "Weapon/RL_Bow.h"
#include "Data/Structs.h"


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

void URL_ProjectileComponent::PullBow(float Damage,FGameplayTag DamageTag,TArray<FFirebalLocation> Locations ,AActor* Target)
{
	if (Bow)
	{
		Bow->SpawnArrow(Damage,DamageTag,Locations,Target);
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
		Bow = SpawnWeapon<>(BowClass);
		
		if (Bow)
		{
			Bow->WeaponOwner=this->GetOwner();
			// 重置弓的相对位置和旋转
			Bow->GetRootComponent()->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator::ZeroRotator);
			Bow->AttachToComponent(AttachCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
		}
	}
	
}

template <typename T>
T* URL_ProjectileComponent::SpawnWeapon(TSubclassOf<T> SpawnClass)
{
	if (SpawnClass)
	{
		return GetWorld()->SpawnActor<T>(SpawnClass);
	}
	return nullptr;
}
