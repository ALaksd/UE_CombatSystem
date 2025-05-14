// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AutomationBlueprintFunctionLibrary.h"
#include "Data/RL_EnemyConfig.h"
#include "RL_AbilitySystemLibrary.generated.h"

class URL_OverlayWidgetController;
class URL_AttributeWidgetController;
class URL_InventoryWidgetController;
class URL_EquipWidgetController;

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_AbilitySystemLibrary : public UAutomationBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static URL_OverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_AttributeWidgetController* GetAttributeMeauController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_InventoryWidgetController* GetInventoryWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_EquipWidgetController* GetEquipWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static URL_LanternFlameController* GetLanternFlameWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void GetLivePlayerWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "Ability|Library", meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "ActorsToIgnore"))
	static void GetLivePlayersInEllipse(
		const UObject* WorldContextObject,
		TArray<AActor*>& OutOverlappingActors,
		const TArray<AActor*>& ActorsToIgnore,
		const FVector& CenterLocation,
		const FVector EllipseRadii,  // X=前向半径，Y=横向半径，Z=垂直半径（可选）
		FRotator Orientation = FRotator::ZeroRotator,
		bool bDrawDebug = false,
		float DebugDuration = 2.0f,
		FColor DebugColor = FColor::Green
	);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	static URL_EnemyConfig* GetEnemyConfig(AActor* Enemy);
};
