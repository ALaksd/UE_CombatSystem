// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueAction.h"

#include "UE_RougelikeDemo/Interact/DialogueUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "UE_RougelikeDemo/Interact/InteractActors/InteractableNPCActor.h"

void UDialogueAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (Cast<AInteractableNPCActor>(InteractedTarget))
	{
		AInteractableNPCActor* NPC = Cast<AInteractableNPCActor>(InteractedTarget);
		UDialogueUserWidget* DialogueWidget = CreateWidget<UDialogueUserWidget>(Interactor->GetWorld(), Widget);
		DialogueWidget->InitWidget(NPC);
		DialogueWidget->AddToViewport();
		NPC->BanAct(Cast<APlayerController>(Cast<ACharacter>(Interactor)->GetController()));
	}
}
