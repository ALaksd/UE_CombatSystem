// Fill out your copyright notice in the Description page of Project Settings.


#include "DieAction.h"

#include "Interface/RL_CombatInterface.h"

void UDieAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (Interactor->Implements<URL_CombatInterface>())
	{
		IRL_CombatInterface::Execute_Die(Interactor);
	}
}
