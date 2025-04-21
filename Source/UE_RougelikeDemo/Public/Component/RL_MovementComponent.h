#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/RL_CharacterData.h"
#include "GameplayTagContainer.h"
#include "RL_MovementComponent.generated.h"

class UInputMappingContext;
class UInputAction;
class URL_InputBufferComponent;
class URLInputConfig;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API URL_MovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URL_MovementComponent();
	// 输入映射上下文和动作的引用
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> BaseIMC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	//跑步按键
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RunAction;

	//翻滚按键
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Settings")
	//角色移动状态数据
	TMap<EMovementState, FMovementSetting> MovementSettingMap;

	UPROPERTY(EditDefaultsOnly,Category = "0_RLCharacter|Input")
	TObjectPtr<URLInputConfig> InputConfig;

	//角色当前移动状态
	UPROPERTY(BlueprintReadWrite)
	EMovementState CurrentMovementState;
	
	// 移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float moveSpeed = 600.0f;

	/***--------------------测试---------------------***/

	void LMBInputPressedTest(FGameplayTag InputTag);
	void LMBInputHeldTest(FGameplayTag InputTag);
	void LMBInputReleasedTest(FGameplayTag InputTag);
	
	/***--------------------测试---------------------***/
	
protected:
	virtual void BeginPlay() override;
	//更新角色移动状态
	UFUNCTION(BlueprintCallable)
	void UpdateMovementState(EMovementState State);
	
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Run(const FInputActionValue& Value);

	void Roll(const FInputActionValue& Value);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// 角色和控制器
	ACharacter* ownerCharacter;
	APlayerController* playerController;
	// 角色移动组件
	UCharacterMovementComponent* characterMovement;
		
};
