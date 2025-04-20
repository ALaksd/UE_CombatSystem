#include "Component/RL_MovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

URL_MovementComponent::URL_MovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URL_MovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// 获取角色和控制器
	ownerCharacter = Cast<ACharacter>(GetOwner());
	if (!ownerCharacter) return;
	playerController = Cast<APlayerController>(ownerCharacter->GetController());
	if (!playerController) return;
	// 将输入映射上下文添加到本地玩家子系统
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	if (InputSubsystem && InputMappingContext)
	{
		InputSubsystem->AddMappingContext(InputMappingContext, 0);
	}

	characterMovement = ownerCharacter->GetCharacterMovement();
	// 设置移动组件速度
	if (characterMovement)
	{
		characterMovement->MaxWalkSpeed = moveSpeed;
		characterMovement->bOrientRotationToMovement = true; // 是否朝向移动方向
		characterMovement->bUseControllerDesiredRotation = false; // 允许控制器控制旋转
		ownerCharacter->bUseControllerRotationYaw = false;
	}

	// 绑定输入动作
	if (UEnhancedInputComponent* EnhancedInputComponent = 
		Cast<UEnhancedInputComponent>(ownerCharacter->InputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(
				MoveAction,
				ETriggerEvent::Triggered,
				this,
				&URL_MovementComponent::Move
			);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(
				LookAction,
				ETriggerEvent::Triggered,
				this,
				&URL_MovementComponent::Look
			);
		}
	}
}

void URL_MovementComponent::Move(const FInputActionValue& Value)
{
	if (!ownerCharacter || !playerController) return;
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator ControlRotation = playerController->GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	ownerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	ownerCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void URL_MovementComponent::Look(const FInputActionValue& Value)
{
	if (!playerController) return;

	FVector2D LookAxis = Value.Get<FVector2D>();
	playerController->AddYawInput(LookAxis.X);
	playerController->AddPitchInput(LookAxis.Y);
}

void URL_MovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

