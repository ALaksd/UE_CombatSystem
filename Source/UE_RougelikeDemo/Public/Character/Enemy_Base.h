// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Interface/RL_CombatInterface.h"
#include "UI/WidgetController/RL_OverlayWidgetController.h"
#include "GameFramework/Character.h"
#include "Enemy_Base.generated.h"

class UBehaviorTree;
class ARL_AIController;
class UWidgetComponent;

UCLASS()
class UE_ROUGELIKEDEMO_API AEnemy_Base : public ACharacter, public IAbilitySystemInterface,public IRL_CombatInterface
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

	/** ComvatInterface */
	virtual UAnimMontage* GetHitReactMotange_Implementation() override;

	/** End ComvatInterface */
	
protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;


	/** AI*/

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BeahabviorTree;

	UPROPERTY()
	TObjectPtr<ARL_AIController> RLAIController;

	/** UI */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	/** Hit */

	//HitReact标签变化回调
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	bool bHitReacting;

	float BaseWalkSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "RLCharacter|AnimMontage")
	TObjectPtr<UAnimMontage> HitReactMontage;

	/** Ability */
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> Abilites;

	void AddCharacterAbilities();

private:
	void InitAbilityActorInfo();

	UPROPERTY(EditDefaultsOnly,Category="Initialize")
	TSubclassOf<UGameplayEffect> PrimariAttribute;
	void InitializeAttribute();
	
};
