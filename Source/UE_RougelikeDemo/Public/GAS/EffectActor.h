// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemComponent.h"
#include "EffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class UE_ROUGELIKEDEMO_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEffectActor();

protected:
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;



protected:
	virtual void BeginPlay() override;

	//Ӧ�ü���Ч��
	UFUNCTION()
	void ApplayEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass);
};
