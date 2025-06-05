// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimationNotify/AN_SpawnProjectile.h"

#include "Component/RL_ProjectileComponent.h"


void UAN_SpawnProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (AActor* Owner = MeshComp->GetOwner())
	{
		if (URL_ProjectileComponent* Com = Owner->FindComponentByClass<URL_ProjectileComponent>())
		{
			if (bIsFireBall)
				Com->PullBow(Damage,Tag);
			else
				Com->PullBow(Damage,Tag,Count,ArcCenterOffset,ArcRadius,ArcAngleDegrees);
		}
	}
}
