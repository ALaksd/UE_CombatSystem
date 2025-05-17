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
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include <Blueprint/AIBlueprintHelperLibrary.h>

// Sets default values
AEnemy_Base::AEnemy_Base()
{
	PrimaryActorTick.bCanEverTick = false;
	
	
	AbilitySystemComponent=CreateDefaultSubobject<UASC_Base>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UAS_Enemy>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetVisibility(false);

	EnemyMovementComponent = CreateDefaultSubobject<URL_EnemyMovementComponent>("EnemyMovementComponent");

	WeaponStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("WeaponStaticMeshComponent");

	RedAttackNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("RedAttackNiagaraComponent");
	RedAttackNiagaraComponent->SetupAttachment(GetMesh());

	PatrolSpline = CreateDefaultSubobject<USplineComponent>("PatrolSpline");
	PatrolSpline->SetupAttachment(GetRootComponent());
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
}

void AEnemy_Base::Execute(bool bIsForward)
{
	if (bIsExecuting) return ;
	
	bIsExecuting = true;
	if (bIsForward)
	{
		float Time = PlayAnimMontage(Aim_Execute_F);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle,[this]()
		{
			// 处决完成,退出破防状态,体力值回满
			bIsGuardBroken=false;
			RemoveTag(FName("EnemyState.GuardBroken"));

			// 回复体力
			FGameplayEffectSpecHandle Handle = AbilitySystemComponent->MakeOutgoingSpec(GE_RestoreStamina,1,AbilitySystemComponent->MakeEffectContext());
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());

			UE_LOG(LogTemp,Warning,TEXT("Execute!!!"));
			GetWorldTimerManager().ClearTimer(GuardBrokenTimer);
			bIsExecuting=false;
		},Time,false);
	}
	else
	{
		float Time = PlayAnimMontage(Aim_Execute_B);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle,[this]()
		{
			// 处决完成,退出破防状态,体力值回满
			bIsGuardBroken=false;
			RemoveTag(FName("EnemyState.GuardBroken"));

			// 回复体力
			FGameplayEffectSpecHandle Handle = AbilitySystemComponent->MakeOutgoingSpec(GE_RestoreStamina,1,AbilitySystemComponent->MakeEffectContext());
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
			
			GetWorldTimerManager().ClearTimer(GuardBrokenTimer);
			bIsExecuting=false;
			
		},Time,false);
	}
}

void AEnemy_Base::TakeDamage(const FGameplayEffectSpecHandle& DamageHandle) const
{
	if (AbilitySystemComponent)
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*DamageHandle.Data.Get());

	//受到伤害后发现敌人
	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get AI Controller"));
		return;
	}

	// 获取Blackboard组件
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Blackboard Component"));
		return;
	}

	// 设置黑板键（示例）
	BlackboardComponent->SetValueAsBool(FName("bFindTarget"), true);
	HealthBar->SetVisibility(true);
}

UAnimMontage* AEnemy_Base::GetHitReactMotange_Implementation()
{
	return EnemyMovementComponent->GetEnemyConfig()->HitReactMontage;
}

void AEnemy_Base::Die_Implementation()
{
	WeaponStaticMeshComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	WeaponStaticMeshComponent->SetSimulatePhysics(true);
	WeaponStaticMeshComponent->SetEnableGravity(true);
	WeaponStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get AI Controller"));
		return;
	}

	// 获取Blackboard组件
	UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing Blackboard Component"));
		return;
	}

	// 设置黑板键（示例）
	BlackboardComponent->SetValueAsBool(FName("bIsDead"), true);
	HealthBar->SetVisibility(false);
	SetLifeSpan(5.f);
}

AActor* AEnemy_Base::GetCombatTarget_Implementation() const
{
	return TargetActor;
}

void AEnemy_Base::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	TargetActor = InCombatTarget;
}

UNiagaraComponent* AEnemy_Base::GetRedAttackNiagaraComponent_Implementation() const
{
	return RedAttackNiagaraComponent;
}

void AEnemy_Base::SetHealthBarVisible_Implementation(bool bVisible) const
{
	HealthBar->SetVisibility(bVisible);
}

void AEnemy_Base::StaminaReduceCallBack()
{
	GetWorldTimerManager().ClearTimer(StaminaReduceTimer);
	GetWorldTimerManager().SetTimer(StaminaReduceTimer,[this]()
	{
		// 回复体力
		FGameplayEffectSpecHandle Handle = AbilitySystemComponent->MakeOutgoingSpec(GE_RestoreStamina,1,AbilitySystemComponent->MakeEffectContext());
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());
	
	},StaminaReduceTime,false);
	
}

void AEnemy_Base::ResilienceReduceCallBack()
{
	GetWorldTimerManager().ClearTimer(ResilienceReduceTimer);
	GetWorldTimerManager().SetTimer(ResilienceReduceTimer,[this]()
	{
		// 回复韧性
		FGameplayEffectSpecHandle Handle = AbilitySystemComponent->MakeOutgoingSpec(GE_RestoreResilience,1,AbilitySystemComponent->MakeEffectContext());
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());

	},ResilienceReduceTime,false);
	
}

void AEnemy_Base::GuardBroken()
{
	AddTag(FName("EnemyState.GuardBroken"));
	bIsGuardBroken=true;
	
	//该状态下敌人无法攻击
	//受到的伤害增加（受到的伤害*1.2)		1
	//并能被处决							1

	GetWorldTimerManager().ClearTimer(GuardBrokenTimer);
	GetWorldTimerManager().SetTimer(GuardBrokenTimer,[this]()
	{
		bIsGuardBroken=false;

		// 回复体力
		FGameplayEffectSpecHandle Handle = AbilitySystemComponent->MakeOutgoingSpec(GE_RestoreStamina,1,AbilitySystemComponent->MakeEffectContext());
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());

		//移除Tag
		RemoveTag(FName("EnemyState.GuardBroken"));
	},GuardBrokenTime,false);
}

void AEnemy_Base::Staggered()
{
	AddTag(FName("EnemyState.Staggered"));
	bIsStaggered=true;

	GetWorldTimerManager().ClearTimer(StaggeredTimer);
	GetWorldTimerManager().SetTimer(StaggeredTimer,[this]()
	{
		bIsStaggered=false;

		// 回复韧性
		FGameplayEffectSpecHandle Handle = AbilitySystemComponent->MakeOutgoingSpec(GE_RestoreResilience,1,AbilitySystemComponent->MakeEffectContext());
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Handle.Data.Get());

		RemoveTag(FName("EnemyState.Staggered"));
	},StaggeredTime,false);
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
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAttributeSet->GetStaminaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnStaminaChanged.Broadcast(Data.NewValue);
			}
		);

		OnHealthChanged.Broadcast(EnemyAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(EnemyAttributeSet->GetMaxHealth());
		OnStaminaChanged.Broadcast(EnemyAttributeSet->GetStamina());

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAttributeSet->GetStaminaAttribute()).AddUObject<AEnemy_Base>(this,&AEnemy_Base::StaminaAttributeChangeCallback);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(EnemyAttributeSet->GetResilienceAttribute()).AddUObject<AEnemy_Base>(this,&AEnemy_Base::ResilienceAttributeChangeCallback);
		
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
	if (EnemyMovementComponent && AbilitySystemComponent)
	{
		FGameplayEffectSpecHandle GameplayEffect = AbilitySystemComponent->MakeOutgoingSpec(EnemyMovementComponent->GetEnemyConfig()->PrimariAttribute,1,AbilitySystemComponent->MakeEffectContext());
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GameplayEffect.Data.Get());
	}
}

void AEnemy_Base::AddTag(FName Tag)
{
	FGameplayTag EnemyTag = FGameplayTag::RequestGameplayTag(Tag);
	StateTags.AddTag(EnemyTag);
	AbilitySystemComponent->AddLooseGameplayTag(EnemyTag);
	AbilitySystemComponent->SetTagMapCount(EnemyTag, 1);
}

void AEnemy_Base::RemoveTag(FName Tag)
{
	FGameplayTag EnemyTag = FGameplayTag::RequestGameplayTag(Tag);
	for (FGameplayTag StateTag : StateTags)
	{
		if (EnemyTag == StateTag)
		{
			StateTags.RemoveTag(StateTag);
			AbilitySystemComponent->RemoveLooseGameplayTag(StateTag);
			AbilitySystemComponent->SetTagMapCount(StateTag, 0);
			break;
		}
	}
}

void AEnemy_Base::StaminaAttributeChangeCallback(const FOnAttributeChangeData& Data)
{
	UE_LOG(LogTemp,Error,TEXT("StaminaAttribute OldValue %f"), Data.OldValue);
	UE_LOG(LogTemp,Warning,TEXT("StaminaAttribute NewValue %f"), Data.NewValue);
	
	if (Data.NewValue < Data.OldValue)
		StaminaReduceCallBack();
	if (Data.NewValue == 0)
	{
		// 进入破防状态
		GuardBroken();
	}
}

void AEnemy_Base::ResilienceAttributeChangeCallback(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue < Data.OldValue)
		ResilienceReduceCallBack();
	if (Data.NewValue == 0)
	{
		// 进入蹒跚状态
		Staggered();
	}
}

void AEnemy_Base::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AEnemy_Base::AddCharacterAbilities()
{
	if (!AbilitySystemComponent) return;

	UASC_Base* ASC = Cast<UASC_Base>(AbilitySystemComponent);
	if (!ASC) return;

	const URL_EnemyConfig* EnemyConfig = EnemyMovementComponent ? EnemyMovementComponent->GetEnemyConfig() : nullptr;
	if (!EnemyConfig) return;

	// 合并所有技能
	TArray<FEnemySkills> AllSkills;
	AllSkills.Append(EnemyConfig->EnemySkills);
	// 调用 ASC 注册技能
	ASC->AddEnemyAbilities(AllSkills);
	ASC->AddCharacterAbilities(Abilites);
}

