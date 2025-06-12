// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/RL_AssetManager.h"
#include "RL_GameplayTags.h"

URL_AssetManager& URL_AssetManager::Get()
{
	check(GEngine);

	URL_AssetManager* RL_AssetManager = Cast<URL_AssetManager>(GEngine->AssetManager);

	return *RL_AssetManager;
}

void URL_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FRLGameplayTags::InitializedNativeGameplayTags();
}
