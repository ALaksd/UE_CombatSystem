// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/RL_EnemySpawnVolumes.h"
#include "Components/BoxComponent.h"
#include <Interface/RL_PlayerInterface.h>
#include "Spawner/RL_EnemySpawnPoint.h"

ARL_EnemySpawnVolumes::ARL_EnemySpawnVolumes()
{
	PrimaryActorTick.bCanEverTick = false;
	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->SetupAttachment(GetRootComponent());

	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 设置碰撞
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore); // 对所有通道响应为 Overlap
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void ARL_EnemySpawnVolumes::ResetEnemySpawn()
{
	bSpawned = false;
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 设置碰撞

}

void ARL_EnemySpawnVolumes::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &ARL_EnemySpawnVolumes::OnBoxOverlap);
	
}

void ARL_EnemySpawnVolumes::OnBoxOverlap(UPrimitiveComponent* OverlapedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

