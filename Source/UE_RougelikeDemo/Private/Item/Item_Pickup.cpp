

#include "Item/Item_Pickup.h"

#include "NiagaraComponent.h"
#include "Character/RL_BaseCharacter.h"
#include "Component/RL_MovementComponent.h"
#include "Components/SphereComponent.h"

AItem_Pickup::AItem_Pickup()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SphereCom = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	this->SetRootComponent(SphereCom);

	NiagaraCom = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraCom->SetupAttachment(SphereCom);
	
}

void AItem_Pickup::BeginPlay()
{
	Super::BeginPlay();

	SphereCom->OnComponentBeginOverlap.AddDynamic(this,&AItem_Pickup::OnCollision);
	SphereCom->OnComponentEndOverlap.AddDynamic(this,&AItem_Pickup::EndCollision);
}

void AItem_Pickup::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	 *	玩家进入物品可拾取范围
	 *
	 *	显示当前可拾取物品信息   :玩家控制部分
	 *	通知玩家,当前可拾取物品	
	 */

	//如果是玩家,将物品给到玩家,由玩家判断物品是否拾取
	if (ARL_BaseCharacter* Player = Cast<ARL_BaseCharacter>(OtherActor))
	{
		Player->MovementComponent->AddItemCanPickup(this);
	}
	
}

void AItem_Pickup::EndCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ARL_BaseCharacter* Player = Cast<ARL_BaseCharacter>(OtherActor))
	{
		Player->MovementComponent->RemoveItemCanPickup(this);
	}
}

