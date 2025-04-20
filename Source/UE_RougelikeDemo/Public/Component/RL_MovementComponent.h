#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RL_MovementComponent.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API URL_MovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URL_MovementComponent();
	// 输入映射上下文和动作的引用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	// 移动速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float moveSpeed = 600.0f;
protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// 角色和控制器
	ACharacter* ownerCharacter;
	APlayerController* playerController;
	// 角色移动组件
	UCharacterMovementComponent* characterMovement;
		
};
