// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor/Interactable_LanternFlame.h"

#include "Blueprint/UserWidget.h"
#include "Character/RL_BaseCharacter.h"
#include "Component/RL_MovementComponent.h"
#include "Components/SphereComponent.h"
#include "UI/Widget/RL_UserWidget.h"

AInteractable_LanternFlame::AInteractable_LanternFlame()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCom = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereCom);

	StaticMeshCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshCom->SetupAttachment(SphereCom);
	
}

void AInteractable_LanternFlame::TryInteract()
{
	//创建UI
	WBP_SavePoint = CreateWidget<URL_UserWidget>(GetWorld(),WBP_SavePointClass);
	WBP_SavePoint->AddToViewport(0);
	WBP_SavePoint->SetVisibility(ESlateVisibility::Visible);
}

void AInteractable_LanternFlame::BeginPlay()
{
	Super::BeginPlay();

	SphereCom->OnComponentBeginOverlap.AddDynamic(this,&AInteractable_LanternFlame::OnComBeginOverlap);
	SphereCom->OnComponentEndOverlap.AddDynamic(this,&AInteractable_LanternFlame::OnComEndOverlap);
}

void AInteractable_LanternFlame::OnComBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARL_BaseCharacter* Player = Cast<ARL_BaseCharacter>(OtherActor))
	{
		Player->FindComponentByClass<URL_MovementComponent>()->AddInteractableActor(this);
	}
}

void AInteractable_LanternFlame::OnComEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ARL_BaseCharacter* Player = Cast<ARL_BaseCharacter>(OtherActor))
	{
		Player->FindComponentByClass<URL_MovementComponent>()->RemoveInteractableActor();
	}
}
