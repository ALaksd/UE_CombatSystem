// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractableActor.h"
#include "InteractableLadderActor.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API AInteractableLadderActor : public AInteractableActor
{
	GENERATED_BODY()

public:

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
};
