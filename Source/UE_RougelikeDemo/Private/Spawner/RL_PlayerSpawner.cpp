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

	// ����һ�� SceneComponent ��Ϊ�����
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// ���� SkeletalMeshComponents �������Ǹ��ӵ������
	SwordMan = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMan"));
	SwordMan->SetupAttachment(RootComponent);  // �� SwordMan ������ӵ������
	SwordMan->SetVisibility(false);

	Archer = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Archer"));
	Archer->SetupAttachment(RootComponent);  // �� Archer ������ӵ������
	Archer->SetVisibility(false);

	Caster = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Caster"));
	Caster->SetupAttachment(RootComponent);  // �� Caster ������ӵ������
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

