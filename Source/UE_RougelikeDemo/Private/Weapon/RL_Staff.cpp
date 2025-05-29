// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RL_Staff.h"

#include "GameplayTagContainer.h"
#include "Weapon/Projectile/RL_Projectile_Fireball.h"

void ARL_Staff::SpawnArrow(float Damage, FGameplayTag DamageTag, float Count, FVector CenterOffset,float Radius, float Angle)
{
	if (Count <= 0)
		return ;

	if (WeaponOwner)
		ArcCenter=WeaponOwner->GetActorLocation();
	
	FireballCount = Count;
	SpawnFireballs(Damage,DamageTag,Count);
}

TArray<ARL_Projectile_Fireball*> ARL_Staff::SpawnFireballs(float Damage, FGameplayTag DamageTag,int32 NumProjectiles)
{
	TArray<ARL_Projectile_Fireball*> Fireballs;
	if (NumProjectiles <= 0) return Fireballs;

	FVector Origin;
	FVector Forward;
	if (WeaponOwner)
	{
		Origin = WeaponOwner->GetActorLocation(); // 火球起点
		Forward = WeaponOwner->GetActorForwardVector();
	}
	else
	{
		Origin = GetActorLocation(); // 火球起点
		Forward = GetActorForwardVector();
	}
	Origin += ArcCenterOffset;
	FVector Up = Origin + FVector(0,0,ArcRadius);

	// 保证法线垂直于 Forward，构造局部坐标系
	FVector Right = FVector::CrossProduct(Up, Forward).GetSafeNormal();
	FVector ArcUp = FVector::CrossProduct(Forward, Right).GetSafeNormal(); // 平面中的Y方向

	// 起始角度设为负的一半圆弧角
	float StartAngle = -ArcAngleDegrees * 0.5f;
	float AngleStep = ArcAngleDegrees / (NumProjectiles - 1);

	for (int i = 0; i < NumProjectiles; ++i)
	{
		float AngleDeg = StartAngle + i * AngleStep;
		float AngleRad = FMath::DegreesToRadians(AngleDeg);

		// 极坐标转换成世界坐标
		FVector Direction = Right * FMath::Cos(AngleRad) + ArcUp * FMath::Sin(AngleRad);
		FVector SpawnLocation = Origin + Direction * ArcRadius;

		// 火球朝向圆心外侧
		FRotator SpawnRotation = Direction.Rotation();

		// 生成火球
		FActorSpawnParameters SpawnParams;
		ARL_Projectile_Fireball* Fireball = GetWorld()->SpawnActor<ARL_Projectile_Fireball>(ArrowClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (Fireball)
		{
			Fireball->InitProjectile(Damage,DamageTag);
			Fireballs.Add(Fireball);
		}
			
	}
	
	return Fireballs;
}


