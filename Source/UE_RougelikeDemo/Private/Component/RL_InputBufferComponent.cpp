// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/RL_InputBufferComponent.h"

URL_InputBufferComponent::URL_InputBufferComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void URL_InputBufferComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void URL_InputBufferComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URL_InputBufferComponent::BufferInput(const FGameplayTag& InputTag)
{
	if (bAcceptingBufferedInput)
	{
		FBufferedInput NewInput;
		NewInput.InputTag = InputTag;
		NewInput.TimeStamp = GetWorld()->GetTimeSeconds();
		InputQueue.Add(NewInput);
		UE_LOG(LogTemp, Log, TEXT("Buffered Input: %s"), *InputTag.ToString());
	}
}

FGameplayTag URL_InputBufferComponent::ConsumeBufferedInput()
{
	if (InputQueue.Num() > 0)
	{
		FGameplayTag ChosenInput = InputQueue.Last().InputTag;
		InputQueue.Empty();
		return ChosenInput;
	}
	return FGameplayTag::EmptyTag;
}

