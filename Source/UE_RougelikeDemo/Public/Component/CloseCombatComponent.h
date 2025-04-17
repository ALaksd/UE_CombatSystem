// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CloseCombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API UCloseCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCloseCombatComponent();


private:
	UPROPERTY()
	TObjectPtr<ACharacter> User = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh = nullptr;

	TArray<FVector> Points;

	// 启动射线检测
	bool bCombat = false;



	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void StartCombat();
	void EndCombat();
		
};
