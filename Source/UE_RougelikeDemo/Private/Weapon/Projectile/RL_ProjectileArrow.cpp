// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile/RL_ProjectileArrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

void ARL_ProjectileArrow::OnProjectileHit(UPrimitiveComponent* HitComponent, const FHitResult& HitResult)
{
	if (!HitComponent) return;

	// 停止投射物移动
	if (ProjectileCom)
	{
		ProjectileCom->StopMovementImmediately();
		ProjectileCom->SetActive(false);
	}

	// 禁用碰撞
	if (SphereCom)
	{
		SphereCom->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 附着到命中的组件
	FAttachmentTransformRules AttachmentRules(
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		EAttachmentRule::KeepWorld,
		true
	);

	// 尝试附着到骨骼（如果是骨骼网格体）
	if (HitResult.BoneName != NAME_None && HitComponent->IsA(USkeletalMeshComponent::StaticClass()))
	{
		AttachToComponent(HitComponent, AttachmentRules, HitResult.BoneName);
	}
	else
	{
		// 附着到静态网格体或其他组件
		AttachToComponent(HitComponent, AttachmentRules);
	}

	// 调整位置和旋转以匹配命中点
	FVector ArrowForward = GetActorForwardVector();
	FVector HitNormal = HitResult.Normal;

	// 计算箭矢应该旋转的角度，使其看起来是"钉"在表面上
	FRotator NewRotation = FRotationMatrix::MakeFromX(-HitNormal).Rotator();
	SetActorRotation(NewRotation);

	// 微调位置，使箭矢看起来插入物体
	SetActorLocation(HitResult.Location + HitNormal * 5.0f);

	StopNiagara();

	// 设置生命周期（可选），例如10秒后自动销毁
	SetLifeSpan(10.0f);
}
