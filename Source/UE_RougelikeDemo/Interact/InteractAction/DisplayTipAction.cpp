// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayTipAction.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Item/Item_Pickup.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_UI.h"

void UDisplayTipAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (TipWidgetClass)
	{
		UUserWidget* TipWidgetInstance = CreateWidget<UUserWidget>(Interactor->GetWorld(), TipWidgetClass);
		if (TipWidgetInstance)
		{
			TipWidgetInstance->AddToViewport();
			UTextBlock* FoundText = Cast<UTextBlock>(TipWidgetInstance->GetWidgetFromName(TEXT("Tip")));
			if (FragmentClass)
			{
				if (AItem_Pickup* Item = Cast<AItem_Pickup>(InteractedTarget))
				{
					if (!Item->ItemInstance) return;

					FText NameText = FText::FromName(Cast<URLItemFragment_UI>(Item->ItemInstance->FindFragmentByClass(FragmentClass))->Name);
					FText FinalText = FText::Format(NSLOCTEXT("UI", "Prompt", "获得 {0} X 1"), NameText);
					FoundText->SetText(FinalText);
				}

			}
			else
			{
				FoundText->SetText(FText::FromString(TEXT("血瓶已升级")));
			}
		}
	}
}
