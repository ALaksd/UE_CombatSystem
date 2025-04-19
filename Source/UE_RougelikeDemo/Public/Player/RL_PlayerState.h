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

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSoulChanged, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelChanged, int32);

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

/// <summary>
/// 玩家等级系统，数据暂时保存在PlayerState里
/// </summary>
public:
	inline int32 GetSoul() const { return Soul; }
	void AddSoul(int32 NewSoul);

	UFUNCTION(BlueprintCallable)
	void SetSoul(int32 NewSoul);

	inline int32 GetLevel() const { return Level; }
	void AddLevel();

	inline int32 GetSpellPoints() const { return SpellPoints; }
	void AddSpellPoints(int32 InSpellPoints);

	UFUNCTION(BlueprintCallable)
	int32 GetLevelRequirement(int32 InLevel);
	FOnSoulChanged OnSoulChanged;
	FOnLevelChanged OnLevelChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable>LevelUpInfo;

	

	int32 Soul = 0;
	int32 Level = 1;
	int32 SpellPoints = 0;
};
