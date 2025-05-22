#include "Armor/ArmorBase.h"

AArmorBase::AArmorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Bone = CreateDefaultSubobject<USkeletalMeshComponent>("ArmorMesh");

}

void AArmorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArmorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

