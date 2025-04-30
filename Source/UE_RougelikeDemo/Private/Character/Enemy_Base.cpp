// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy_Base.h"
#include "GAS/ASC_Base.h"
#include "GAS/AS/AS_Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/RL_AIController.h"
#include "BehaviorTree/BehaviorTree.h"

// Sets default values
AEnemy_Base::AEnemy_Base()
{
	PrimaryActorTick.bCanEverTick = false;
	
	//创建组件
	// Capsule=CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	// SetRootComponent(Capsule);
	//
	// SkeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	// SkeletalMesh->SetupAttachment(GetRootComponent());
	
	AbilitySystemComponent=CreateDefaultSubobject<UASC_Base>("AbilitySystemComponent");

	AttributeSet = CreateDefaultSubobject<UAS_Enemy>("AttributeSet");

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
}

void AEnemy_Base::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

}

void AEnemy_Base::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	RLAIController = Cast<ARL_AIController>(NewController);
	RLAIController->GetBlackboardComponent()->InitializeBlackboard(*BeahabviorTree->BlackboardAsset);
	RLAIController->RunBehaviorTree(BeahabviorTree);
}

void AEnemy_Base::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UASC_Base>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeAttribute();
	
}

void AEnemy_Base::InitializeAttribute()
{
	if (PrimariAttribute && AbilitySystemComponent)
	{
		FGameplayEffectSpecHandle GameplayEffect = AbilitySystemComponent->MakeOutgoingSpec(PrimariAttribute,1,AbilitySystemComponent->MakeEffectContext());
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffect.Data.Get());
	}
}

