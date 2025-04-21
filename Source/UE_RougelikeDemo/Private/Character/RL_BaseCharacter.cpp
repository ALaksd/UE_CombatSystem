// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RL_BaseCharacter.h"
#include"Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/RL_InputBufferComponent.h"
#include "UI/RL_PlayerStateWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/CloseCombatComponent.h"
#include "GAS/ASC_Base.h"
#include "Input/RLInputComponent.h"
#include "Interface/RL_CharacterAimInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/RL_HUD.h"




// Sets default values
ARL_BaseCharacter::ARL_BaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true; //????????????????????????????????

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FllowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FllowCamera->SetupAttachment(CameraBoom);
	FllowCamera->bUsePawnControlRotation = false;

	InputBufferComponent = CreateDefaultSubobject<URL_InputBufferComponent>("InputBufferComponnet");

	CloseCombatComponent = CreateDefaultSubobject<UCloseCombatComponent>(TEXT("CloseCombatComponent"));
	
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

void ARL_BaseCharacter::AddCharacterAbilities()
{
	//if (!HasAuthority()) return;
	UASC_Base* ASC = CastChecked<UASC_Base>(AbilitySystemComponent);

	ASC->AddCharacterAbilities(StartupAbilities);
	ASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
	
}

// Called when the game starts or when spawned
void ARL_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//??????????????????????????
	// if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	// {
	// 	if (UEnhancedInputLocalPlayerSubsystem* System = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	// 	{
	// 		System->AddMappingContext(BaseIMC, 0);
	// 	}
	// }

	//????????UI
	if (PlayerStateUIClass)
	{
		PlayerStateUI = CreateWidget<URL_PlayerStateWidget>(GetWorld(), PlayerStateUIClass);
		if (PlayerStateUI)
		{
			PlayerStateUI->AddToViewport();
		}
	}

	PlayerState = Cast<ARL_PlayerState>(GetPlayerState());


	//SetupPlayerInputComponent(UGameplayStatics::GetPlayerController(GetWorld(),0)->InputComponent);
}


// Called every frame
void ARL_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARL_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Super::SetupPlayerInputComponent(PlayerInputComponent);
	// if (URLInputComponent* RLInputComponent = CastChecked<URLInputComponent>(PlayerInputComponent))
	// {
	// 	RLInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	//
	// 	RLInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	//
	// 	RLInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARL_BaseCharacter::Move);
	//
	// 	RLInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARL_BaseCharacter::Look);
	// 	
	// 	RLInputComponent->BindAbilityInputAction(InputConfig,this,&ThisClass::LMBInputPressedTest,&ThisClass::LMBInputReleasedTest,&ThisClass::LMBInputHeldTest);
	//
	// }

}





/***--------------------≤‚ ‘---------------------***/

// void ARL_BaseCharacter::LMBInputPressedTest(FGameplayTag InputTag)
// {
// 	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Pressed")));
// }
//
// void ARL_BaseCharacter::LMBInputHeldTest(FGameplayTag InputTag)
// {
// 	//GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Blue, FString::Printf(TEXT("Held")));
// 	CastChecked<UASC_Base>(AbilitySystemComponent)->AbilityInputTagHeld(InputTag);
//
// 	//ª∫¥Ê‘§ ‰»Î
// 	InputBufferComponent->BufferInput(InputTag);
// }
//
// void ARL_BaseCharacter::LMBInputReleasedTest(FGameplayTag InputTag)
// {
// 	//GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Green, FString::Printf(TEXT("Released")));
// 	CastChecked<UASC_Base>(AbilitySystemComponent)->AbilityInputTagReleased(InputTag);
// }
/***--------------------≤‚ ‘---------------------***/






// void ARL_BaseCharacter::Move(const FInputActionValue& Value)
// {
// 	FVector2D MoveVector = Value.Get<FVector2D>();
//
// 	if (Controller != nullptr)
// 	{
// 		const FRotator Rotation = Controller->GetControlRotation();
// 		const FRotator YawRotation(0, Rotation.Yaw, 0);
//
// 		// get forward vector
// 		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
//
// 		// get right vector 
// 		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
//
// 		// add movement 
// 		AddMovementInput(ForwardDirection, MoveVector.Y);
// 		AddMovementInput(RightDirection, MoveVector.X);
// 	}
// }

// void ARL_BaseCharacter::Look(const FInputActionValue& Value)
// {
// 	FVector2D LookAxisVector = Value.Get<FVector2D>();
//
// 	if (Controller != nullptr)
// 	{
// 		// add yaw and pitch input to controller
// 		AddControllerYawInput(LookAxisVector.X);
// 		AddControllerPitchInput(LookAxisVector.Y);
// 	}
// }
//
// void ARL_BaseCharacter::Run(const FInputActionValue& Value)
// {
//
// }
//
// void ARL_BaseCharacter::Roll(const FInputActionValue& Value)
// {
//
// }

void ARL_BaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//???????????????????? ????????????
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void ARL_BaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//???????????????????? ????????
	InitAbilityActorInfo();
}

void ARL_BaseCharacter::InitAbilityActorInfo()
{
	if(!PlayerState)
		PlayerState = Cast<ARL_PlayerState>(GetPlayerState());
	check(PlayerState);

	PlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerState, this);
	AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	Cast<UASC_Base>(PlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AttributeSet = PlayerState->GetAttributeSet();
	//≥ı ºªØHUD
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (ARL_HUD* RLGHUD = Cast<ARL_HUD>(PlayerController->GetHUD()))
		{
			RLGHUD->InitOverlay(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeAttribute();
}

void ARL_BaseCharacter::InitializeAttribute() const
{
	ApplyGameEffect(DefaultPrimaryAttributes,1.f);
	ApplyGameEffect(DefaultSecondaryAttributes, 1.f);
	ApplyGameEffect(DefaultVitalAttributes, 1.f);
}

void ARL_BaseCharacter::ApplyGameEffect(TSubclassOf<UGameplayEffect> EffectClass, float Level) const
{
	if (GetAbilitySystemComponent() && EffectClass)
	{
		const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
	}
}

// void ARL_BaseCharacter::UpdateMovementState(EMovementState State)
// {
// 	FMovementSetting* MovementSettingPtr = MovementSettingMap.Find(State);
// 	if(MovementSettingPtr)
// 	{
// 		CurrentMovmentState = State;
// 		GetCharacterMovement()->MaxWalkSpeed = MovementSettingPtr->MaxWalkSpeed;
// 		GetCharacterMovement()->MaxAcceleration = MovementSettingPtr->MaxAcceleration;
// 		GetCharacterMovement()->BrakingDecelerationWalking = MovementSettingPtr->BrakingDeceleration;
// 		GetCharacterMovement()->BrakingFrictionFactor = MovementSettingPtr->BrakingFrictionFactor;
// 		GetCharacterMovement()->BrakingFriction = MovementSettingPtr->BrakingFriction;
// 		GetCharacterMovement()->bUseSeparateBrakingFriction = MovementSettingPtr->bUseSeparateBrakingFriction;
//
// 		if (GetMesh()->GetAnimInstance()->Implements<URL_CharacterAimInterface>())
// 		{
// 			IRL_CharacterAimInterface::Execute_ReciveMovementState(GetMesh()->GetAnimInstance(),State);
// 		}
// 	}
// }

int32 ARL_BaseCharacter::GetSoul_Implementation() const
{
	return PlayerState->GetSoul();
}

void ARL_BaseCharacter::SetSoul_Implementation(int32 NewSoul)
{
	PlayerState->SetSoul(NewSoul);
}

int32 ARL_BaseCharacter::GetLevel_Implementation() const
{
	return PlayerState->GetLevel();
}

void ARL_BaseCharacter::AddLevel_Implementation()
{
	PlayerState->AddLevel();
}

int32 ARL_BaseCharacter::GetSpellPoints_Implementation() const
{
	return PlayerState->GetSpellPoints();
}

void ARL_BaseCharacter::AddSpellPoints_Implementation(int32 InSpellPoints)
{
	PlayerState->AddSpellPoints(InSpellPoints);
}

int32 ARL_BaseCharacter::GetLevelRequirement_Implementation(int32 InLevel) const
{
	return PlayerState->GetLevelRequirement(InLevel);
}
