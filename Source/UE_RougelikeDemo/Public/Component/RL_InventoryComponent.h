// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RL_InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API URL_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URL_InventoryComponent();


protected:
	virtual void BeginPlay() override;

};
