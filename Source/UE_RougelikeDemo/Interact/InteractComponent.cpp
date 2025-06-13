
#include "InteractComponent.h"

#include "InteractableActor.h"
#include "InteractableInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

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

void UInteractComponent::OnInteractTargetChanged(AActor* InteractableActor)
{
	if (InteractableActor && Cast<AInteractableActor>(InteractableActor)->IsTrigger)
	{
		return;	
	}
	
	if (InteractableActor && InteractableActor->Implements<UInteractableInterface>())
	{
		// 初始化 Widget
		if (!InteractWidgetInstance && InteractWidgetClass)
		{
			InteractWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), InteractWidgetClass);
			if (InteractWidgetInstance)
			{
				InteractWidgetInstance->AddToViewport();

				// 你可以通过 Widget 中暴露的接口获取文字控件，比如绑定变量 InteractText
				UTextBlock* FoundText = Cast<UTextBlock>(InteractWidgetInstance->GetWidgetFromName(TEXT("Tip")));
				if (FoundText)
				{
					InteractTextBlock = FoundText;
				}
			}
		}

		// 更新提示文字
		FText HintText = Cast<AInteractableActor>(InteractableActor)->GetInteractHintText();
		if (InteractTextBlock)
		{
			InteractTextBlock->SetText(HintText);
		}

		if (InteractWidgetInstance)
		{
			InteractWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		// 没有目标时隐藏 UI
		if (InteractWidgetInstance)
		{
			InteractWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

