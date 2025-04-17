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
#include "RL_BaseCharacter.generated.h"

class URLInputConfig;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class URL_PlayerStateWidget;
class URL_InputBufferComponent;
class UCloseCombatComponent;



UCLASS()
class UE_ROUGELIKEDEMO_API ARL_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	
protected:


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FllowCamera;

	//攻击组件
	UPROPERTY(EditDefaultsOnly,Category = "Component")
	TObjectPtr<UCloseCombatComponent> CloseCombatComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<URL_InputBufferComponent> InputBufferComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|UI")
	TSubclassOf<UUserWidget> PlayerStateUIClass;

#pragma region

	///*外用骨骼*/
	//UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> HeadConstant;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Shoulder;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Hair;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Boots;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Chest;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Back;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Hands;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Helmet;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Legs;

	/**
	 * 角色输入以及移动相关的设置，这些后面可能会优化到一个组件里面
	 */
#pragma endregion //外用骨骼

#pragma region


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> BaseIMC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	//跑步按键
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RunAction;

	//翻滚按键
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RollAction;
#pragma endregion //输入

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Settings")
	//角色移动状态数据
	TMap<EMovementState, FMovementSetting> MovementSettingMap;

	//角色当前移动状态
	UPROPERTY(BlueprintReadWrite)
	EMovementState CurrentMovmentState;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditDefaultsOnly,Category = "Input")
	TObjectPtr<URLInputConfig> InputConfig;
	
	
	
	/***--------------------测试---------------------***/

	void LMBInputPressedTest(FGameplayTag InputTag);
	void LMBInputHeldTest(FGameplayTag InputTag);
	void LMBInputReleasedTest(FGameplayTag InputTag);
	
	/***--------------------测试---------------------***/
	
	
	
	
private:
	//角色状态UI
	TObjectPtr<URL_PlayerStateWidget> PlayerStateUI;

	TObjectPtr<ARL_PlayerState> PlayerState;
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly , Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;


public:
	// Sets default values for this character's properties
	ARL_BaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//FORCEINLINE 定义强制内联函数，适用于多次调用的简单函数，减少函数调用开销，提高代码效率
	FORCEINLINE class USpringArmComponent* GetSpringArmComponent() { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetCameraComponent() { return FllowCamera; }


	inline UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }

	//初始化玩家技能列表
	void AddCharacterAbilities();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//更新角色移动状态
	UFUNCTION(BlueprintCallable)
	void UpdateMovementState(EMovementState State);
	
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Run(const FInputActionValue& Value);

	void Roll(const FInputActionValue& Value);

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	void InitAbilityActorInfo();
	void InitializePrimaryAttribute() const;
};

