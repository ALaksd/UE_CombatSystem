// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RL_BaseCharacter.h"
#include"Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/RL_PlayerStateWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interface/RL_CharacterAimInterface.h"



// Sets default values
ARL_BaseCharacter::ARL_BaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true; //使用控制器所需的旋转

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FllowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FllowCamera->SetupAttachment(CameraBoom);
	FllowCamera->bUsePawnControlRotation = false;

	//HeadConstant->CreateDefaultSubobject<USkeletalMeshComponent>("HeadConstant");
	//HeadConstant->SetupAttachment(GetMesh());
	//HeadConstant->SetLeaderPoseComponent(GetMesh());

	//Shoulder->CreateDefaultSubobject<USkeletalMeshComponent>("Shoulder");
	//Shoulder->SetupAttachment(GetMesh());
	//Shoulder->SetLeaderPoseComponent(GetMesh());

	//Hair->CreateDefaultSubobject<USkeletalMeshComponent>("Hair");
	//Hair->SetupAttachment(GetMesh());
	//Hair->SetLeaderPoseComponent(GetMesh());

	//Boots->CreateDefaultSubobject<USkeletalMeshComponent>("Boots");
	//Boots->SetupAttachment(GetMesh());
	//Boots->SetLeaderPoseComponent(GetMesh());

	//Chest->CreateDefaultSubobject<USkeletalMeshComponent>("Chest");
	//Chest->SetupAttachment(GetMesh());
	//Chest->SetLeaderPoseComponent(GetMesh());

	//Back->CreateDefaultSubobject<USkeletalMeshComponent>("Back");
	//Back->SetupAttachment(GetMesh());
	//Back->SetLeaderPoseComponent(GetMesh());

	//Hands->CreateDefaultSubobject<USkeletalMeshComponent>("Hands");
	//Hands->SetupAttachment(GetMesh());
	//Hands->SetLeaderPoseComponent(GetMesh());

	//Helmet->CreateDefaultSubobject<USkeletalMeshComponent>("Helmet");
	//Helmet->SetupAttachment(GetMesh());
	//Helmet->SetLeaderPoseComponent(GetMesh());

	//Legs->CreateDefaultSubobject<USkeletalMeshComponent>("Legs");
	//Legs->SetupAttachment(GetMesh());
	//Legs->SetLeaderPoseComponent(GetMesh());

}

// Called when the game starts or when spawned
void ARL_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//添加增强输入系统
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* System = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			System->AddMappingContext(BaseIMC, 0);
		}
	}

	//添加UI
	if (PlayerStateUIClass)
	{
		PlayerStateUI = CreateWidget<URL_PlayerStateWidget>(GetWorld(), PlayerStateUIClass);
		if (PlayerStateUI)
		{
			PlayerStateUI->AddToViewport();
		}
	}

	PlayerState = Cast<ARL_PlayerState>(GetPlayerState());

}


// Called every frame
void ARL_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARL_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARL_BaseCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARL_BaseCharacter::Look);
	}

}

void ARL_BaseCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MoveVector.Y);
		AddMovementInput(RightDirection, MoveVector.X);
	}
}

void ARL_BaseCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARL_BaseCharacter::Run(const FInputActionValue& Value)
{

}

void ARL_BaseCharacter::Roll(const FInputActionValue& Value)
{

}

void ARL_BaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//初始化角色状态
	InitAbilityActorInfo();

}

void ARL_BaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//初始化角色状态
	InitAbilityActorInfo();
}

void ARL_BaseCharacter::InitAbilityActorInfo()
{
	if(!PlayerState)
		PlayerState = Cast<ARL_PlayerState>(GetPlayerState());
	check(PlayerState);

	PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, this);
	AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	AttributeSet = PlayerState->GetAttributeSet();

	IniltializePrimaryAttribute();
}

void ARL_BaseCharacter::IniltializePrimaryAttribute() const
{
	if (GetAbilitySystemComponent() && DefaultPrimaryAttributes)
	{
		const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle);
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
	}
}

void ARL_BaseCharacter::UpdateMovementState(EMovementState State)
{
	FMovementSetting* MovementSettingPtr = MovementSettingMap.Find(State);
	if(MovementSettingPtr)
	{
		CurrentMovmentState = State;
		GetCharacterMovement()->MaxWalkSpeed = MovementSettingPtr->MaxWalkSpeed;
		GetCharacterMovement()->MaxAcceleration = MovementSettingPtr->MaxAcceleration;
		GetCharacterMovement()->BrakingDecelerationWalking = MovementSettingPtr->BrakingDeceleration;
		GetCharacterMovement()->BrakingFrictionFactor = MovementSettingPtr->BrakingFrictionFactor;
		GetCharacterMovement()->BrakingFriction = MovementSettingPtr->BrakingFriction;
		GetCharacterMovement()->bUseSeparateBrakingFriction = MovementSettingPtr->bUseSeparateBrakingFriction;

		if (GetMesh()->GetAnimInstance()->Implements<URL_CharacterAimInterface>())
		{
			IRL_CharacterAimInterface::Execute_ReciveMovementState(GetMesh()->GetAnimInstance(),State);
		}
	}
}