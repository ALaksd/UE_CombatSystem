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
#include "Component/RL_MovementComponent.h"
#include "GAS/ASC_Base.h"
#include "Input/RLInputComponent.h"
#include "Interface/RL_CharacterAimInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/RL_HUD.h"
#include "Components/CapsuleComponent.h"



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

	//创建玩家输入组件
	MovementComponent = CreateDefaultSubobject<URL_MovementComponent>(TEXT("PlayerInputComponent"));

	//添加Tag
	Tags.Add(FName("Player"));
	

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

}


// Called every frame
void ARL_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


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
	//初始化HUD
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

UAnimMontage* ARL_BaseCharacter::GetHitReactMotange_Implementation()
{
	return HitReactMontage;
}

bool ARL_BaseCharacter::isDead_Implementation() const
{
	return false;
}

AActor* ARL_BaseCharacter::GetAvatar_Implementation()
{
	return this;
}

void ARL_BaseCharacter::Die_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
