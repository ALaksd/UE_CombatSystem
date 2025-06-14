// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayNormalUI.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"

void UDisplayNormalUI::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (Cast<ACharacter>(Interactor))
	{
		UUserWidget* InteractWidgetInstance = CreateWidget<UUserWidget>(Interactor->GetWorld(), InteractWidgetClass);
		if (InteractWidgetInstance)
		{
			InteractWidgetInstance->AddToViewport();
		}
	}
}
