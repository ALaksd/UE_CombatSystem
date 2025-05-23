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
class URLInventoryComponent_Equipment;
class URLInventoryComponent;


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
	//大背包
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
	TObjectPtr<URLInventoryComponent> InventoryComponent;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<URLInventoryComponent_Equipment> EquipmentInventoryComponnet;
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

	//重置属性
	void ResetAttribute();


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

	//等级
	inline int32 GetLevel() const { return Level; }
	void AddLevel();

	//技能点
	inline int32 GetSpellPoints() const { return SpellPoints; }
	void AddSpellPoints(int32 InSpellPoints);

	UFUNCTION(BlueprintCallable)
	int32 GetLevelRequirement(int32 InLevel);
	FOnSoulChanged OnSoulChanged;
	FOnLevelChanged OnLevelChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable>LevelUpInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TSubclassOf<UGameplayEffect> InitializeAttributeEffect;


	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void UnBindAllDelegates();
	

	int32 Soul = 0;
	// 玩家等级
	int32 Level = 1;
	// 可用技能点
	int32 SpellPoints = 0;
};
