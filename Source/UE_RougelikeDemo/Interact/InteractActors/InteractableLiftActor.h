// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractableActor.h"
#include "InteractableLiftActor.generated.h"

class UTimelineComponent;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API AInteractableLiftActor : public AInteractableActor
{
	GENERATED_BODY()

public:

	AInteractableLiftActor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void LiftRunning(FVector Offset);

	UPROPERTY(EditAnywhere)
	bool IsUp;

	UPROPERTY(EditAnywhere)
	float Time;

protected:
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LiftMesh;

	UPROPERTY(EditAnywhere, Category = "Lift")
	UCurveFloat* TranslateCurve;

	UPROPERTY()
	UTimelineComponent* UpTimeline;

	UPROPERTY()
	UTimelineComponent* DownTimeline;

	FVector UpLocation;
	FVector DownLocation;

	UFUNCTION()
	void HandleTimelineProgressUp(float Value);

	UFUNCTION()
	void HandleTimelineProgressDown(float Value);

	UFUNCTION()
	void OnTimelineFinished();
};
