// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UE_RougelikeDemo/Interact/InteractableActor.h"
#include "InteractableNPCActor.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API AInteractableNPCActor : public AInteractableActor
{
	GENERATED_BODY()

public:

	AInteractableNPCActor();
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* NPCMesh;
	
	// 对话内容
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> DialogueContent;

	// 数组下标
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> BaseIMC;

	//移动操作
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> MoveIMC;

	UFUNCTION(BlueprintCallable)
	void BanAct(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void FreeAct(APlayerController* PlayerController);
};
