#include "InteractableActor.h"

#include "InteractActionBase.h"
#include "InteractComponent.h"
#include "InteractionDataAsset.h"
#include "Character/RL_BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractCollision"));
	RootComponent = InteractCollision;

	InteractCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	InteractCollision->SetGenerateOverlapEvents(true);
}

void AInteractableActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARL_BaseCharacter* Player = Cast<ARL_BaseCharacter>(OtherActor))
	{
		if (IsTrigger)
		{
			Interact_Implementation(Player);
			return;
		}
		if (UInteractComponent* InteractComp = Player->FindComponentByClass<UInteractComponent>())
		{
			InteractComp->RegisterInteractable(this);
		}
	}
}

void AInteractableActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ARL_BaseCharacter* Player = Cast<ARL_BaseCharacter>(OtherActor))
	{
		if (IsTrigger)
		{
			return;
		}
		if (UInteractComponent* InteractComp = Player->FindComponentByClass<UInteractComponent>())
		{
			InteractComp->UnregisterInteractable(this);
		}
	}
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	InteractCollision->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::OnOverlapBegin);
	InteractCollision->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::OnOverlapEnd);
}

void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractableActor::Interact_Implementation(AActor* Interactor)
{
	if (InteractionData)
	{
		for (UInteractActionBase* Action : InteractionData->InteractActions)
		{
			if (Action)
			{
				Action->Execute(Interactor, this);
			}
		}
	}
}

