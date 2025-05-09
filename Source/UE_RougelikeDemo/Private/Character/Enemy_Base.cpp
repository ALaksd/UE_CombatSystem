// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy_Base.h"
#include "GAS/ASC_Base.h"
#include "GAS/AS/AS_Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/RL_AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include <UI/Widget/RL_UserWidget.h>
#include "Components/WidgetComponent.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "Components/SplineComponent.h"

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

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	EnemyMovementComponent = CreateDefaultSubobject<URL_EnemyMovementComponent>("EnemyMovementComponent");

	PatrolSpline = CreateDefaultSubobject<USplineComponent>("PatrolSpline");
	PatrolSpline->SetupAttachment(GetRootComponent());
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
}

UAnimMontage* AEnemy_Base::GetHitReactMotange_Implementation()
{
	return HitReactMontage;
}

AActor* AEnemy_Base::GetCombatTarget_Implementation() const
{
	return TargetActor;
}

void AEnemy_Base::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	TargetActor = InCombatTarget;
}

void AEnemy_Base::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

	//设置自己为HealthBar的控制器
	if (URL_UserWidget* RL_UserWidget = Cast<URL_UserWidget>(HealthBar->GetUserWidgetObject()))
	{
		RL_UserWidget->SetWidgetController(this);
	}
	//绑定回调函数
	if (const UAS_Enemy* EnemyAttributeSet = Cast<UAS_Enemy>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(EnemyAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(EnemyAttributeSet->GetMaxHealth());
	}

	//绑定标签变化
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("Effect.HitReact")),EGameplayTagEventType::NewOrRemoved).AddUObject
	(this, &AEnemy_Base::HitReactTagChanged);
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
	
	AddCharacterAbilities();
}

void AEnemy_Base::InitializeAttribute()
{
	if (PrimariAttribute && AbilitySystemComponent)
	{
		FGameplayEffectSpecHandle GameplayEffect = AbilitySystemComponent->MakeOutgoingSpec(PrimariAttribute,1,AbilitySystemComponent->MakeEffectContext());
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffect.Data.Get());
	}
}

void AEnemy_Base::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AEnemy_Base::AddCharacterAbilities()
{
	//if (!HasAuthority()) return;
	UASC_Base* ASC = CastChecked<UASC_Base>(AbilitySystemComponent);

	ASC->AddCharacterAbilities(Abilites);

}