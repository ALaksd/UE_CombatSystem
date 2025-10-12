// Fill out your copyright notice in the Description page of Project Settings.


#include "Interact_PlaySound.h"

#include "Kismet/GameplayStatics.h"

void UInteract_PlaySound::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	UGameplayStatics::PlaySound2D(Interactor, Sound);
}
