// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/EffectActor.h"
#include <AbilitySystemInterface.h>

// Sets default values
AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneComponent"));
}

// Called when the game starts or when spawned
void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEffectActor::ApplayEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass)
{
	if (!TargetActor)
		return;

	IAbilitySystemInterface* AbilitySystemInteface = Cast<IAbilitySystemInterface>(TargetActor);
	if (AbilitySystemInteface)
	{
		UAbilitySystemComponent* AbilitySystemComponent = AbilitySystemInteface->GetAbilitySystemComponent();

		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		const FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(EffectClass,1.f, EffectContext);
		
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

}


