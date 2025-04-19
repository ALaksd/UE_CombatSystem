// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "RL_AssetManager.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_AssetManager : public UAssetManager
{
	GENERATED_BODY()
public:

	URL_AssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
