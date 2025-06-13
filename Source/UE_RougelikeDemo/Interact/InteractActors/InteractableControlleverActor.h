// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractableActor.h"
#include "InteractableControlleverActor.generated.h"

class AInteractableLiftActor;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API AInteractableControlleverActor : public AInteractableActor
{
	GENERATED_BODY()

public:

	AInteractableControlleverActor();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* LeverMesh;

	UPROPERTY(EditDefaultsOnly)
	UAnimSequence* PushAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimSequence* PullAnim;

	UPROPERTY(EditDefaultsOnly)
	AInteractableLiftActor* Lift;

	UPROPERTY(EditDefaultsOnly)
	bool IsUp;

	UPROPERTY(EditDefaultsOnly)
	bool IsPush;
};
