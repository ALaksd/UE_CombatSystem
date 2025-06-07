// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/RL_Projectile_Fireball.h"

#include "GameFramework/ProjectileMovementComponent.h"

ARL_Projectile_Fireball::ARL_Projectile_Fireball()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARL_Projectile_Fireball::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsInDistance)
		UpdateTracking(DeltaSeconds);
}

void ARL_Projectile_Fireball::UpdateTracking(float DeltaSeconds)
{
	if (!Target) return;
	
	TimeSinceLastTracking += DeltaSeconds;
	if (TimeSinceLastTracking >= TimeSinceLastTrackingUpdate)
	{
		// 计算方向
		FVector NewDir = (Target->GetActorLocation()-this->GetActorLocation());

		// 如果两者距离太短,取消追踪能力
		if (NewDir.Length() <= MaxTrackingDistance)
		{
			bIsInDistance = true;
			return ;
		}

		
		// 应用
		ApplyDir(NewDir.GetSafeNormal());
	}
}

void ARL_Projectile_Fireball::ApplyDir(FVector Direction)
{
	FVector Froward = ProjectileCom->Velocity.GetSafeNormal();
	
	// 计算当前方向与目标方向之间的角度
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Froward, Direction)));

	if (Angle > MaxTrackingAngle)
	{
		/// 限制修正角度
		// 计算最大可接受的目标方向
		FVector Axis = FVector::CrossProduct(Froward, Direction).GetSafeNormal();
		FQuat RotationQuat = FQuat(Axis, FMath::DegreesToRadians(MaxTrackingAngle));
		Direction = RotationQuat.RotateVector(Froward);
	}
	else
	{
		// 应用追踪强度插值
		Direction = FMath::Lerp(Froward, Direction, TrackingStrength);
	}
	// 确保方向向量单位化
	Direction.Normalize();

	// 修改速度方向
	ProjectileCom->Velocity = Direction*ProjectileCom->Velocity.Size();
	
}
