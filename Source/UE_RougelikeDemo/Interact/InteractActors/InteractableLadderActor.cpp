// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableLadderActor.h"

#include "Character/RL_BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void AInteractableLadderActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (ARL_BaseCharacter* Player = Cast<ARL_BaseCharacter>(OtherActor))
	{
		UCharacterMovementComponent* MovementComponent = Player->GetCharacterMovement();

		MovementComponent->SetMovementMode(MOVE_Walking);
		MovementComponent->bOrientRotationToMovement = true;

	}
}
