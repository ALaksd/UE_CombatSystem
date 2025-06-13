// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractableActor.h"
#include "InteractableDoorActor.generated.h"

class UTimelineComponent;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API AInteractableDoorActor : public AInteractableActor
{
	GENERATED_BODY()
public:
	AInteractableDoorActor();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void PushDoor();

	UFUNCTION(BlueprintCallable)
	void TranslateDoor(FVector Offset);

	UPROPERTY(EditAnywhere)
	AInteractableActor* PassCheck;

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorLeftMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorRightMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, Category = "Door")
	UCurveFloat* OpenCurve;

	UPROPERTY()
	UTimelineComponent* OpenTimeline;

	FRotator InitialLRotation;
	FRotator TargetLRotation;

	FRotator InitialRRotation;
	FRotator TargetRRotation;

	FVector InitialLocation;
	FVector TargetLocation;

	UFUNCTION()
	void HandleTimelineProgress1(float Value);

	UFUNCTION()
	void HandleTimelineProgress2(float Value);
};
