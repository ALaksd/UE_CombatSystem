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
#include <System/RL_SavePointSubsystem.h>
#include "System/RL_SanitySubsystem.h"

#include "Components/BoxComponent.h"
#include <Interface/RL_PlayerInterface.h>
#include "Spawner/RL_EnemySpawnPoint.h"

AInteractable_LanternFlame::AInteractable_LanternFlame()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMeshCom = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshCom->SetupAttachment(InteractCollision);

	SKM1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKM1"));
	SKM1->SetupAttachment(StaticMeshCom);

	SKM2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKM2"));
	SKM2->SetupAttachment(StaticMeshCom);

	SKM3 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKM3"));
	SKM3->SetupAttachment(StaticMeshCom);

	SKM4 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKM4"));
	SKM4->SetupAttachment(StaticMeshCom);

	DIZUO = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DIZUO"));
	DIZUO->SetupAttachment(StaticMeshCom);

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(GetRootComponent());

	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 设置碰撞
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore); // 对所有通道响应为 Overlap
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
}

void AInteractable_LanternFlame::ResetEnemySpawn()
{
	for (auto EnemyPoint : SpawnPoints)
	{
		if (EnemyPoint)
		{
			EnemyPoint->DestroyEnemy();
		}
	}
	bSpawned = false;
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 设置碰撞
}

void AInteractable_LanternFlame::ActivatePoint()
{
	bIsActive = true;

	// 注册存档点数据
	if (URL_SavePointSubsystem* SaveSystem = GetGameInstance()->GetSubsystem<URL_SavePointSubsystem>())
	{
		FSavePointData NewData;
		NewData.PointID = FName(GetFName());
		NewData.DisplayName = FText::FromString(LanternFlameName); // 添加自定义显示名称变量
		NewData.bActive = true;
		NewData.Location = FVector(GetActorLocation().X,GetActorLocation().Y,GetActorLocation().Z + 150.f);
		NewData.Rotation = GetActorRotation();
		NewData.MapName = FName(GetWorld()->GetMapName());
		NewData.LanternFlamePtr = this;

		SaveSystem->RegisterSavePoint(NewData);
		SaveSystem->SetCurrentSavaPoint(NewData.PointID);
	}

	OnPointActivaete();
}

void AInteractable_LanternFlame::OnBoxOverlap(UPrimitiveComponent* OverlapedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<URL_PlayerInterface>()) return;

	if (!bSpawned)
	{
		for (ARL_EnemySpawnPoint* SpawnPoint : SpawnPoints)
		{
			if (IsValid(SpawnPoint))
			{
				SpawnPoint->SpawnEnemy();
			}
		}
	}
	bSpawned = true;
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AInteractable_LanternFlame::BeginPlay()
{
	Super::BeginPlay();

	PlayerController=Cast<ARL_BasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));

	Box->OnComponentBeginOverlap.AddDynamic(this, &AInteractable_LanternFlame::OnBoxOverlap);

	SkeletalToAnimMap.Add(SKM1, Anims[0]);
	SkeletalToAnimMap.Add(SKM2, Anims[1]);
	SkeletalToAnimMap.Add(SKM3, Anims[2]);
	SkeletalToAnimMap.Add(SKM4, Anims[3]);
	SkeletalToAnimMap.Add(DIZUO, Anims[4]);

}