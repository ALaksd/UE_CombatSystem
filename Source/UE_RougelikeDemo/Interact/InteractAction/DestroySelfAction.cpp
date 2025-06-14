// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroySelfAction.h"

void UDestroySelfAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	InteractedTarget->Destroy();
}