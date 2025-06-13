// Fill out your copyright notice in the Description page of Project Settings.


#include "DisplayUI.h"

#include "Kismet/GameplayStatics.h"

void UDisplayUI::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	URL_UIManagerSubsystem* UIManagerSubsystem = Interactor->GetGameInstance()->GetSubsystem<URL_UIManagerSubsystem>();
	UIManagerSubsystem->AddNewWidget(InteractWidgetClass, UGameplayStatics::GetPlayerController(InteractedTarget, 0));
}
