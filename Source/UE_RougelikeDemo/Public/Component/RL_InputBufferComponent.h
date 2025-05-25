// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RL_InputBufferComponent.generated.h"

USTRUCT(BlueprintType)
struct FBufferedInput
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	float TimeStamp = 0.f;
};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBufferedInputConsumed,FGameplayTag,ConsumeTag)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API URL_InputBufferComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URL_InputBufferComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void BufferInput(const FGameplayTag& InputTag);
	void SetbAcceptingBufferedInput(bool Newbool) { bAcceptingBufferedInput = Newbool; }
	bool GetbAcceptingBufferedInput() {return bAcceptingBufferedInput;}
	//外部调用，在通知结束时调用
	FGameplayTag ConsumeBufferedInput();

protected:
	virtual void BeginPlay() override;		
private:
	TArray<FBufferedInput> InputQueue;
	bool bAcceptingBufferedInput = false;
	float BufferWindowDuration = 0.2f;
};
