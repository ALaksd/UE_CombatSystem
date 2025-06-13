

#include "Item/Item_Pickup.h"

#include "NiagaraComponent.h"
#include "Character/RL_BaseCharacter.h"
#include "Component/RL_MovementComponent.h"
#include "Components/SphereComponent.h"

AItem_Pickup::AItem_Pickup()
{
	PrimaryActorTick.bCanEverTick = false;
	
	IdleEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleEffectComponent"));
	IdleEffectComponent->SetupAttachment(RootComponent);
	
}

void AItem_Pickup::BeginPlay()
{
	Super::BeginPlay();

	if (IdleEffect && IdleEffectComponent)
	{
		IdleEffectComponent->SetTemplate(IdleEffect);
		IdleEffectComponent->ActivateSystem();
	}

}
