
#include "InteractComponent.h"

#include "InteractableInterface.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractComponent::TryInteract()
{
	AActor* Target = GetBestInteractable();
	if (Target && Target->Implements<UInteractableInterface>())
	{
		IInteractableInterface::Execute_Interact(Target, GetOwner());
	}
}

void UInteractComponent::RegisterInteractable(AActor* InteractableActor)
{
	if (!InteractableActor || !InteractableActor->Implements<UInteractableInterface>())
		return;

	OverlappingInteractables.AddUnique(InteractableActor);
	OnInteractTargetChanged(GetBestInteractable());
}

void UInteractComponent::UnregisterInteractable(AActor* InteractableActor)
{
	OverlappingInteractables.Remove(InteractableActor);
	OnInteractTargetChanged(GetBestInteractable());
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

}

AActor* UInteractComponent::GetBestInteractable() const
{
	for (TWeakObjectPtr<AActor> ActorPtr : OverlappingInteractables)
	{
		if (ActorPtr.IsValid())
		{
			return ActorPtr.Get(); // 暂时只取第一个有效的
		}
	}
	return nullptr;
}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

