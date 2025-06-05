// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/Enemy/ANS_SpawnProjectile.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/RL_ProjectileComponent.h"
#include "GAS/RL_AbilitySystemLibrary.h"
#include <Data/Enums.h>


void UANS_SpawnProjectile::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (URL_ProjectileComponent* Com = Owner->FindComponentByClass<URL_ProjectileComponent>())
		{
			if (!bIsFireBall)
				Com->PullBow(Damage,Tag);
			else
			{
				if (ACharacter* Character = Cast<ACharacter>(Owner))
				{
					if (AAIController* AIController = Cast<AAIController>(Character->GetController()))
					{
						if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
						{
							AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(FName("TargetToFollow")));
							Com->PullBow(Damage,Tag,Locations,Target);
						}
					}
				}
			}
		}
	}
}

void UANS_SpawnProjectile::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
		if (bIsFireBall && bIsShowDebug)
			ShowFireballLocation(Owner);
}

void UANS_SpawnProjectile::ShowFireballLocation(AActor* Owner)
{
	TArray<FFirebalLocation> SpawnLocations;
	FVector Origin = FVector::ZeroVector;

	for (FFirebalLocation& SpawnLocation : Locations)
	{
		FFirebalLocation NewLocation =  SpawnLocation;
		NewLocation.Location+=FVector(0,0,180);
		SpawnLocations.Add(SpawnLocation);
	}

	for (FFirebalLocation& Location : Locations)
	{
		TArray<FHitResult> Hits;
		TArray<AActor*> ActorsToIgnore;
		FVector RectangleParam = FVector(5, 5, 5);
		float ShpereRadius = 5.f;
		URL_AbilitySystemLibrary::GetLivePlayersInArea(
			Owner,
			Hits,
			ActorsToIgnore,
			Location.Location,
			RectangleParam,
			ShpereRadius,
			FRotator::ZeroRotator,
			EDetectionShapeType::Sphere,
			true,   
			10.f,
			FColor::Green
		);
	}
}
