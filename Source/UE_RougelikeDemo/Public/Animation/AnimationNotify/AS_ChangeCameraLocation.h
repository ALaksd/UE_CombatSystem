// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AS_ChangeCameraLocation.generated.h"

/**
 * 
 */

enum class E_CameraLocation:uint8;


UCLASS()
class UE_ROUGELIKEDEMO_API UAS_ChangeCameraLocation : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,Category = "Arrtibute")
	E_CameraLocation CameraLocation;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
