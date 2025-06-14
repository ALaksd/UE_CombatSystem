// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractActionBase.h"
#include "UE_RougelikeDemo/Interact/InteractableActor.h"
#include "ReplaceActorAction.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UReplaceActorAction : public UInteractActionBase
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AInteractableActor> NewActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnRotation;

	virtual void Execute(AActor* Interactor, AActor* InteractedTarget) override;
};
