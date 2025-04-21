#include "Component/RL_MovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/RL_InputBufferComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"
#include "GAS/ASC_Base.h"
#include "Input/RLInputComponent.h"
#include "Interface/RL_CharacterAimInterface.h"

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
	if (InputSubsystem && BaseIMC)
	{
		InputSubsystem->AddMappingContext(BaseIMC, 0);
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
	if (URLInputComponent* RLInputComponent = CastChecked<URLInputComponent>(ownerCharacter->InputComponent))
	{
		RLInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &URL_MovementComponent::Move);

		RLInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &URL_MovementComponent::Look);
		
		RLInputComponent->BindAbilityInputAction(InputConfig,this,&ThisClass::LMBInputPressedTest,&ThisClass::LMBInputReleasedTest,&ThisClass::LMBInputHeldTest);
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

void URL_MovementComponent::UpdateMovementState(EMovementState State)
{
	FMovementSetting* MovementSettingPtr = MovementSettingMap.Find(State);
	if(MovementSettingPtr)
	{
		CurrentMovementState = State;
		ownerCharacter->GetCharacterMovement()->MaxWalkSpeed = MovementSettingPtr->MaxWalkSpeed;
		ownerCharacter->GetCharacterMovement()->MaxAcceleration = MovementSettingPtr->MaxAcceleration;
		ownerCharacter->GetCharacterMovement()->BrakingDecelerationWalking = MovementSettingPtr->BrakingDeceleration;
		ownerCharacter->GetCharacterMovement()->BrakingFrictionFactor = MovementSettingPtr->BrakingFrictionFactor;
		ownerCharacter->GetCharacterMovement()->BrakingFriction = MovementSettingPtr->BrakingFriction;
		ownerCharacter->GetCharacterMovement()->bUseSeparateBrakingFriction = MovementSettingPtr->bUseSeparateBrakingFriction;

		if (ownerCharacter->GetMesh()->GetAnimInstance()->Implements<URL_CharacterAimInterface>())
		{
			IRL_CharacterAimInterface::Execute_ReciveMovementState(ownerCharacter->GetMesh()->GetAnimInstance(),State);
		}
	}
}

void URL_MovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void URL_MovementComponent::LMBInputPressedTest(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Pressed")));
}

void URL_MovementComponent::LMBInputHeldTest(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Blue, FString::Printf(TEXT("Held")));
	CastChecked<UASC_Base>(ownerCharacter->GetPlayerState()->FindComponentByClass<UAbilitySystemComponent>())->AbilityInputTagHeld(InputTag);

	//缓存预输入
	ownerCharacter->FindComponentByClass<URL_InputBufferComponent>()->BufferInput(InputTag);
}

void URL_MovementComponent::LMBInputReleasedTest(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Green, FString::Printf(TEXT("Released")));
	CastChecked<UASC_Base>(ownerCharacter->GetPlayerState()->FindComponentByClass<UAbilitySystemComponent>())->AbilityInputTagReleased(InputTag);
}