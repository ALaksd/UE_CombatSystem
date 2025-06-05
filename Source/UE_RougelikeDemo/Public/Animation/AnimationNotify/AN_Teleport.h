// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Teleport.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNotifyReceived, USkeletalMeshComponent*, MeshComp, UAnimSequenceBase*, Animation);

/**
 * 触发瞬移
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UAN_Teleport : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnNotifyReceived OnNotifyReceived;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
