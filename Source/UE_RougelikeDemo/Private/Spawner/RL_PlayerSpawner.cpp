// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/RL_PlayerSpawner.h"
#include "RL_CharacterSelectionWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Character/RL_BaseCharacter.h"


// Sets default values
ARL_PlayerSpawner::ARL_PlayerSpawner()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 创建一个 SceneComponent 作为根组件
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// 创建 SkeletalMeshComponents 并将它们附加到根组件
	SwordMan = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMan"));
	SwordMan->SetupAttachment(RootComponent);  // 将 SwordMan 组件附加到根组件
	SwordMan->SetVisibility(false);

	Archer = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Archer"));
	Archer->SetupAttachment(RootComponent);  // 将 Archer 组件附加到根组件
	Archer->SetVisibility(false);

	Caster = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Caster"));
	Caster->SetupAttachment(RootComponent);  // 将 Caster 组件附加到根组件
	Caster->SetVisibility(false);

}


// Called when the game starts or when spawned
void ARL_PlayerSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (CharacterSelectionWidgetClass)
	{
		CharacterSelectionWidget = CreateWidget<URL_CharacterSelectionWidget>(GetWorld(), CharacterSelectionWidgetClass);
		if (CharacterSelectionWidget)
		{
			CharacterSelectionWidget->CharacterSelection = this;
			CharacterSelectionWidget->AddToViewport();
		}
	}
	
}

void ARL_PlayerSpawner::SelectCharacter(int Index)
{
	switch (Index)
	{
	case 1:
	{
		SwordMan->SetVisibility(true);
		Archer->SetVisibility(false);
		Caster->SetVisibility(false);
		SelectedCharacterType = ECharacterType::Warrior;
		break;
	}
	case 2:
	{
		SwordMan->SetVisibility(false);
		Archer->SetVisibility(true);
		Caster->SetVisibility(false);
		SelectedCharacterType = ECharacterType::Archer;
		break;
	}
	case 3:
	{
		SwordMan->SetVisibility(false);
		Archer->SetVisibility(false);
		Caster->SetVisibility(true);
		SelectedCharacterType = ECharacterType::Caster;
		break;
	}
	default: return;
	}
}

