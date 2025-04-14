// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GAS/AS/AS_Player.h"
#include "GameplayEffectTypes.h"
#include "Data\RL_CharacterData.h"
#include "RL_PlayerState.generated.h"




class UDataTable;


/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API ARL_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	

public:
	ARL_PlayerState();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAS_Player> AttributeSet;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Data")
	TObjectPtr<UDataTable> DT_ChracterAttribute;

private:

#pragma region 
	//����ֵ
	UPROPERTY(Replicated)
	FGameplayAttributeData Health;
	//����
	UPROPERTY(Replicated)
	FGameplayAttributeData Endurance;
	//����ֵ
	UPROPERTY(Replicated)
	FGameplayAttributeData Momentum;

#pragma endregion //����
	
	
/// <summary>
/// ����
/// </summary>
public:
	inline virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override{return AbilitySystemComponent;}
	inline UAttributeSet* GetAttributeSet() const{return AttributeSet;}


protected:
	virtual void PostInitializeComponents()override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:

#pragma region 
	void OnRep_Health(const FGameplayAttributeData& OldHealth);
	void OnRep_Endurance(const FGameplayAttributeData& OldHealth);
	void OnRep_Momentum(const FGameplayAttributeData& OldHealth);
#pragma endregion //���Ա任�ص�����

};
