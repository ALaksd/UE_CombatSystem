#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/RL_CharacterData.h"
#include "GameplayTagContainer.h"
#include "RL_MovementComponent.generated.h"

class AInteractable_Base;
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

	//移动操作
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> MoveIMC;

	//其他操作,如切换武器
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> OtherIMC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	//跑步按键
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "0_RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RunAction;

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

	/***--------------------测试---------------------***/

	void LMBInputPressedTest(FGameplayTag InputTag);
	void LMBInputHeldTest(FGameplayTag InputTag);
	void LMBInputReleasedTest(FGameplayTag InputTag);
	
	/***--------------------测试---------------------***/
	
protected:
	virtual void BeginPlay() override;
	
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Run(const FInputActionValue& Value);

	void RunOver(const FInputActionValue& Value);
	
	void Collect(const FInputActionValue& Value);

	//切换锁定敌人（可能需要更改）

	/** 锁定/取消锁定目标 */

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// 角色和控制器
	ACharacter* ownerCharacter;
	APlayerController* playerController;
	// 角色移动组件
	UCharacterMovementComponent* characterMovement;

	/***--------------------交互相关---------------------***/
	
	// 进入物品可拾取回调
	void AddItemCanPickup(AItem_Pickup* ItemToPickup_T);
	// 退出物品可拾取范围回调函数
	void RemoveItemCanPickup(AItem_Pickup* ItemToPickup_T);

	void AddInteractableActor(AInteractable_Base* InteractableActor_T);
	void RemoveInteractableActor();
	
	//更新角色移动状态
	UFUNCTION(BlueprintCallable)
	void UpdateMovementState(EMovementState State);

	//镜头锁定
	/** 锁定/取消锁定目标 */
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void ToggleLockOn();

	/** 搜索锁定目标 */
	UFUNCTION(BlueprintCallable, Category = "LockOn")
	void FindLockOnTarget();
	
private:
	TArray<AItem_Pickup*> ItemsCanPickup;
	AItem_Pickup* ItemToPickup;
	// 计算两个向量间的夹角,返回角度
	float CalculateAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB);

	AInteractable_Base* InteractableActor;
	/***--------------------交互相关---------------------***/

	//锁定镜头相关
	UPROPERTY(EditAnywhere, Category = "LockOn")
	AActor* CurrentTarget;
	
	UPROPERTY(EditAnywhere, Category = "LockOn")
	TArray<AActor*> LockableTargets;
	
	int32 CurrentTargetIndex;
	
	UPROPERTY(EditAnywhere, Category = "LockOn")
	float LockOnRadius = 1000.f;

	UPROPERTY(EditAnywhere, Category = "LockOn")
	FName LockableTag = FName("Lockable");

	UPROPERTY(EditAnywhere, Category = "LockOn")
	FName PlayerLockingTag = FName("IsLocking");
	
	void UpdateLockOnRotation(float DeltaTime);

	void SwitchTargetLeft();

	void SwitchTargetRight();
};
