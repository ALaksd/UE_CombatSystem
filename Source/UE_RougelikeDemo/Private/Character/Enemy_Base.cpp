// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy_Base.h"

#include "GAS/ASC_Base.h"
#include "GAS/AS/AS_Enemy.h"

// Sets default values
AEnemy_Base::AEnemy_Base()
{
	PrimaryActorTick.bCanEverTick = false;
	
	//创建组件
	Capsule=CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);
	
	SkeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMesh->SetupAttachment(GetRootComponent());
	
	AbilitySystemComponent=CreateDefaultSubobject<UASC_Base>("AbilitySystemComponent");

	AttributeSet = CreateDefaultSubobject<UAS_Enemy>("AttributeSet");

	
}

void AEnemy_Base::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();

}

void AEnemy_Base::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UASC_Base>(AbilitySystemComponent)->AbilityActorInfoSet();

	// if (GetAbilitySystemComponent() && DefaultPrimaryAttributes)
	// {
	// 	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	// 	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1.f, ContextHandle);
	// 	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
	// }
	
}

