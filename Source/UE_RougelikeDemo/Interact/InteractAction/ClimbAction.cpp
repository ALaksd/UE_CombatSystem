// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbAction.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UClimbAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (ACharacter* Character = Cast<ACharacter>(Interactor))
	{
		UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();

		MovementComponent->SetMovementMode(MOVE_Flying);
		MovementComponent->bOrientRotationToMovement = false;
		
	}
}
