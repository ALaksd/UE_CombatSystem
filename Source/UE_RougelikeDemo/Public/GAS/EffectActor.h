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
	//瞬间效果
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	//持续Buff
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;



protected:
	virtual void BeginPlay() override;

	//Ӧ�ü���Ч��
	UFUNCTION()
	void ApplayEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass);
};
