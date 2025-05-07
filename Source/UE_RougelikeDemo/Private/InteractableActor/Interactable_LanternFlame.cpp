// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor/Interactable_LanternFlame.h"

#include "Blueprint/UserWidget.h"
#include "Character/RL_BaseCharacter.h"
#include "Component/RL_MovementComponent.h"
#include "Components/SphereComponent.h"
#include "Controller/RL_BasePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/RL_HUD.h"
#include "UI/Widget/RL_UserWidget.h"
#include "UI/WidgetController/RL_LanternFlameController.h"
#include <System/RL_UIManagerSubsystem.h>

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
	if (ARL_HUD* RLHUD = Cast<ARL_HUD>(PlayerController->GetHUD()))
	{
		if (URL_LanternFlameController* LanternFlameWidgetController = RLHUD->GetLanternFlameWidgetController())
		{
			// 初始化数据
			LanternFlameWidgetController->Initialize(SkillList);
			
			
			UGameInstance* GameInstance = GetWorld()->GetGameInstance();
			if (GameInstance)
			{
				URL_UIManagerSubsystem* UIManager = GameInstance->GetSubsystem<URL_UIManagerSubsystem>();
				if (UIManager)
				{
					WBP_SavePoint = UIManager->AddNewWidget(WBP_SavePointClass, UGameplayStatics::GetPlayerController(this, 0));
				}
			}

			// 初始化UI
			WBP_SavePoint->SetWidgetController(LanternFlameWidgetController);
			LanternFlameWidgetController->BroadcastInitialValue();
			InitPointName();
			
			//WBP_SavePoint->AddToViewport(0);
			//WBP_SavePoint->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AInteractable_LanternFlame::BeginPlay()
{
	Super::BeginPlay();

	SphereCom->OnComponentBeginOverlap.AddDynamic(this,&AInteractable_LanternFlame::OnComBeginOverlap);
	SphereCom->OnComponentEndOverlap.AddDynamic(this,&AInteractable_LanternFlame::OnComEndOverlap);

	PlayerController=Cast<ARL_BasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
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
