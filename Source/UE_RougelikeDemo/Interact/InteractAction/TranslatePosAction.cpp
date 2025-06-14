// Fill out your copyright notice in the Description page of Project Settings.


#include "TranslatePosAction.h"
#include "GameFramework/Character.h"

void UTranslatePosAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (Cast<ACharacter>(Interactor))
	{
		Interactor->SetActorLocation(EndPos);
	}
}
