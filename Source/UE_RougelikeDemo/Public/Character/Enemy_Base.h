// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Enemy_Base.generated.h"

class UBehaviorTree;
class ARL_AIController;

UCLASS()
class UE_ROUGELIKEDEMO_API AEnemy_Base : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AEnemy_Base();


private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	//属性
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeSet> AttributeSet;




public:
	inline  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return AbilitySystemComponent;}

	
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;


	/** AI*/

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BeahabviorTree;

	UPROPERTY()
	TObjectPtr<ARL_AIController> RLAIController;

private:
	void InitAbilityActorInfo();

	UPROPERTY(EditDefaultsOnly,Category="Initialize")
	TSubclassOf<UGameplayEffect> PrimariAttribute;
	void InitializeAttribute();
	
};
