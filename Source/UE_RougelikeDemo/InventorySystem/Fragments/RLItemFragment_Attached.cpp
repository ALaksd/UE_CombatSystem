// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_Attached.h"

AActor* URLItemFragment_Attached::AttachToActor(AActor* Owner) const
{
	// 检查是否指定了正确的 AttachedActorClass
	if (!AttachedActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Actor Class specified for attachment."));
		return nullptr;
	}

	AActor* AttachedActor = Owner->GetWorld()->SpawnActor<AActor>(AttachedActorClass, Owner->GetActorLocation(), FRotator::ZeroRotator);
	if (!AttachedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to spawn attached actor."));
		return nullptr;
	}

	// 尝试获取目标插槽，附加到物体上
	USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(Owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	if (SkeletalMeshComp && !AttchedSokectName.IsNone())
	{
		// 使用 AttachToComponent 将附加的 Actor 绑定到插槽上
		EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
		EAttachmentRule RotationRule = EAttachmentRule::SnapToTarget;
		EAttachmentRule ScaleRule = EAttachmentRule::KeepRelative;
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(LocationRule, RotationRule, ScaleRule, true);
		AttachedActor->AttachToComponent(SkeletalMeshComp, AttachmentTransformRules, AttchedSokectName);

		AttachedActor->SetActorRelativeLocation(FVector::ZeroVector);
		AttachedActor->SetActorRelativeRotation(FRotator::ZeroRotator);

		return AttachedActor;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent or socket name not found on the ItemActor."));
		return nullptr;
	}
}

