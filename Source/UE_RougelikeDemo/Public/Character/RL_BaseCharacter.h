// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/RL_CharacterData.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "InputActionValue.h"
#include "Player/RL_PlayerState.h"
#include "Interface/RL_PlayerInterface.h"
#include "Interface/RL_CombatInterface.h"
#include "RL_BaseCharacter.generated.h"

class URL_MovementComponent;
class URLInputConfig;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class URL_PlayerStateWidget;
class URL_InputBufferComponent;
class UCloseCombatComponent;



UCLASS()
class UE_ROUGELIKEDEMO_API ARL_BaseCharacter : public ACharacter, public IAbilitySystemInterface,public IRL_PlayerInterface,public IRL_CombatInterface
{
	GENERATED_BODY()

	
public:

	/** PlayerInterfece */
	virtual int32 GetSoul_Implementation() const override;
	virtual void SetSoul_Implementation(int32 NewSoul) override;
	virtual int32 GetLevel_Implementation() const override;
	virtual void AddLevel_Implementation() override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void AddSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetLevelRequirement_Implementation(int32 InLevel) const override;
	
	/** End PlayerInterfece  */

	/** CombatInterface */
	virtual UAnimMontage* GetHitReactMotange_Implementation() override;
	inline  UAnimMontage* GetLightHitReactFrontMontage_Implementation() const override { return HitReactFrontMontage; }
	inline  UAnimMontage* GetLightHitReactBackMontage_Implementation() const override { return HitReactBackMontage; }
	inline  UAnimMontage* GetLightHitReactLeftMontage_Implementation() const override { return HitReactFLeftMontage; }
	inline  UAnimMontage* GetLightHitReactRightMontage_Implementation() const override { return HitReactRightMontage; }
	inline  UAnimMontage* GetHeavyHitReactMontage_Implementation() const override { return HitReactHeavyFrontMontage; }

	virtual bool isDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual void Die_Implementation() override;
	virtual void KnockBack_Implementation(const FVector& KonckBackImpulse) override;
	/** End CombatInterface */

public:
	//输入组件
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Component")
	TObjectPtr<URL_MovementComponent> MovementComponent;
	
protected:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FllowCamera;
	
	//攻击组件
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Component")
	TObjectPtr<UCloseCombatComponent> CloseCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<URL_InputBufferComponent> InputBufferComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|UI")
	TSubclassOf<UUserWidget> PlayerStateUIClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Attribute")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Attribute")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Attribute")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(EditDefaultsOnly,Category = "RLCharacter|Input")
	TObjectPtr<URLInputConfig> InputConfig;
	
	UPROPERTY(EditDefaultsOnly, Category = "RLCharacter|AnimMontage")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "RLCharacter|AnimMontage")
	TObjectPtr<UAnimMontage> HitReactFrontMontage;

	UPROPERTY(EditDefaultsOnly, Category = "RLCharacter|AnimMontage")
	TObjectPtr<UAnimMontage> HitReactBackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "RLCharacter|AnimMontage")
	TObjectPtr<UAnimMontage> HitReactRightMontage;

	UPROPERTY(EditDefaultsOnly, Category = "RLCharacter|AnimMontage")
	TObjectPtr<UAnimMontage> HitReactFLeftMontage;

	UPROPERTY(EditDefaultsOnly, Category = "RLCharacter|AnimMontage")
	TObjectPtr<UAnimMontage> HitReactHeavyFrontMontage;

	UPROPERTY(BlueprintReadWrite)
	bool bIsDead;

	UFUNCTION(BlueprintImplementableEvent)
	void OnDead();
private:
	//角色状态UI
	TObjectPtr<URL_PlayerStateWidget> PlayerStateUI;

	TObjectPtr<ARL_PlayerState> PlayerState;
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly , Category = "0_RLCharacter|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	//用于存放一些开始状态激活一次永久存在的技能
	UPROPERTY(EditDefaultsOnly, Category = "0_RLCharacter|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
public:
	// Sets default values for this character's properties
	ARL_BaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//FORCEINLINE 定义强制内联函数，适用于多次调用的简单函数，减少函数调用开销，提高代码效率
	FORCEINLINE class USpringArmComponent* GetSpringArmComponent() { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetCameraComponent() { return FllowCamera; }


	inline UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }

	//初始化玩家技能列表
	void AddCharacterAbilities();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	void InitAbilityActorInfo();
	void InitializeAttribute() const;
	void ApplyGameEffect(TSubclassOf<UGameplayEffect> EffectClass,float Level) const;

	//重生
	void ReStart();
};

