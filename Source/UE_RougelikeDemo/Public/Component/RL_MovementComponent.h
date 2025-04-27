#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/RL_CharacterData.h"
#include "GameplayTagContainer.h"
#include "RL_MovementComponent.generated.h"

class AItem_Pickup;
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

	//拾取按键
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> CollectAction;

	//锁定敌人按键
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LockAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> STLAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> STRAction;

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
	
	void Collect(const FInputActionValue& Value);

	//切换锁定敌人（可能需要更改）
	void SwitchTargetLeft(const FInputActionValue& Value);

	void SwitchTargetRight(const FInputActionValue& Value);

	/** 锁定/取消锁定目标 */
	void ToggleLockOn(const FInputActionValue& Value);

	void UpdateLockOnRotation(float DeltaTime);

	/** 搜索锁定目标 */
	void FindLockOnTarget();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// 角色和控制器
	ACharacter* ownerCharacter;
	APlayerController* playerController;
	// 角色移动组件
	UCharacterMovementComponent* characterMovement;

	/***--------------------物品拾取---------------------***/
	
	// 进入物品可拾取回调
	void AddItemCanPickup(AItem_Pickup* ItemToPickup_T);
	// 退出物品可拾取范围回调函数
	void RemoveItemCanPickup(AItem_Pickup* ItemToPickup_T);
private:
	TArray<AItem_Pickup*> ItemsCanPickup;
	AItem_Pickup* ItemToPickup;
	// 计算两个向量间的夹角,返回角度
	float CalculateAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB);

	/***--------------------物品拾取---------------------***/

	/*** 锁定镜头相关 ***/
	UPROPERTY(EditAnywhere, Category = "LockOn")
	AActor* CurrentTarget;

	/** 当前可切换的锁定目标列表 */
	UPROPERTY(EditAnywhere, Category = "LockOn")
	TArray<AActor*> LockableTargets;

	/** 当前锁定目标在列表中的索引 */
	int32 CurrentTargetIndex;

	/** 搜索半径 */
	UPROPERTY(EditAnywhere, Category = "LockOn")
	float LockOnRadius = 1000.f;

	/** 锁定状态 */
	bool bIsLockedOn;

};
