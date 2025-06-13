// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/RL_EnemyMovementComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/StaticMeshSocket.h"
#include <Character/Enemy_Base.h>

URL_EnemyMovementComponent::URL_EnemyMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void URL_EnemyMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner())
	{
		PatrolSpline = GetOwner()->FindComponentByClass<USplineComponent>();
		AActor* OwnerActor = GetOwner();
		USkeletalMeshComponent* CharacterMesh = OwnerActor->FindComponentByClass<USkeletalMeshComponent>();

		//设置骨骼网格体和动画蓝图
		if (EnemyConfig && EnemyConfig->LoadEnemySkeletalMesh() && EnemyConfig->EnemyAnimInstanceClass)
		{
			CharacterMesh->SetSkeletalMesh(EnemyConfig->LoadEnemySkeletalMesh());
			CharacterMesh->SetAnimInstanceClass(EnemyConfig->EnemyAnimInstanceClass);
		}

		const UStaticMesh* WeaponMeshAsset = GetEnemyConfig() ? GetEnemyConfig()->LoadWeaponStaticMesh() : nullptr;
		if (WeaponMeshAsset)
		{
			// 创建静态网格体组件
			WeaponMeshComponent = Cast<AEnemy_Base>(GetOwner())->GetWeaponStaticComponnent();
		
			if (WeaponMeshComponent)
			{
				WeaponMeshComponent->SetStaticMesh(const_cast<UStaticMesh*>(WeaponMeshAsset));
				WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
				WeaponMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
				WeaponMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

				// 附加到角色的骨骼网格体上
				if (CharacterMesh)
				{
					const FName SocketName = GetEnemyConfig()->WeaponAttachSocket;
					WeaponMeshComponent->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
				}
			}
		}
	}
}




void URL_EnemyMovementComponent::InitializePatrolPoints(USplineComponent* NewSpline)
{
	PatrolSpline = NewSpline;
	PatrolPoints.Empty();
	const int32 NumPoints = PatrolSpline->GetNumberOfSplinePoints();

	for (int32 i = 0; i < NumPoints; ++i)
	{
		PatrolPoints.Add(PatrolSpline->GetLocationAtSplinePoint(
			i,
			ESplineCoordinateSpace::World // 使用世界坐标系
		));
	}
}

void URL_EnemyMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

FTransform URL_EnemyMovementComponent::GetWeaopnSocketTransform()
{
	return WeaponMeshComponent->GetSocketTransform(EnemyConfig->WeaponTipSocket);

}


