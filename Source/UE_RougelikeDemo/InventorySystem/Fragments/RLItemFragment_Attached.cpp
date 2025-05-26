// Fill out your copyright notice in the Description page of Project Settings.


#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_Attached.h"
#include <Interface/RL_ItemInstanceHolder.h>

#include "Armor/ArmorBase.h"
#include "GameFramework/Character.h"

AActor* URLItemFragment_Attached::AttachToActor(AActor* Owner_T, URLInventoryItemInstance* ItemInstance) const
{
	AActor* AttachedActor = nullptr;
	// 检查是否指定了正确的 AttachedActorClass
	for (auto Pair : AttachedMap)
	{
		if (!Pair.Value)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Actor Class specified for attachment."));
			return nullptr;
		}
		AttachedActor = Owner_T->GetWorld()->SpawnActor<AActor>(Pair.Value, Owner_T->GetActorLocation(), FRotator::ZeroRotator);
		if (!AttachedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn attached actor."));
			return nullptr;
		}

		IRL_ItemInstanceHolder::Execute_SetItemInstance(AttachedActor, ItemInstance);

		// 尝试获取目标插槽，附加到物体上
		USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(Owner_T->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		if (SkeletalMeshComp && !Pair.Key.IsNone())
		{
			// 使用 AttachToComponent 将附加的 Actor 绑定到插槽上
			EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
			EAttachmentRule RotationRule = EAttachmentRule::SnapToTarget;
			EAttachmentRule ScaleRule = EAttachmentRule::KeepRelative;
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(LocationRule, RotationRule, ScaleRule, true);
			AttachedActor->AttachToComponent(SkeletalMeshComp, AttachmentTransformRules, Pair.Key);

			AttachedActor->SetActorRelativeLocation(FVector::ZeroVector);
			AttachedActor->SetActorRelativeRotation(FRotator::ZeroRotator);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent or socket name not found on the ItemActor."));
			return nullptr;
		}
	}
	return AttachedActor;
}

URLInventoryItemInstance* URLItemFragment_Attached::ArmorAttachToActor(AActor* Owner_T, URLInventoryItemInstance* ItemInstance) const
{
	AActor* AttachedActor = nullptr;
	USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(Owner_T->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	TArray<AActor*> AttachedActors;
	SkeletalMeshComp->GetOwner()->GetAttachedActors(AttachedActors);
	
	// 检查是否指定了正确的 AttachedActorClass
	for (auto Pair : AttachedMap)
	{
		if (!Pair.Value)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Actor Class specified for attachment."));
			return nullptr;
		}
		AttachedActor = Owner_T->GetWorld()->SpawnActor<AActor>(Pair.Value, Owner_T->GetActorLocation(), FRotator::ZeroRotator);
		if (!AttachedActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn attached actor."));
			return nullptr;
		}
		for (auto ChildActor : AttachedActors)
		{
			if (ChildActor)
			{
				// 检查该 Actor 是否附加到目标插槽
				FName AttachedSocketName = ChildActor->GetAttachParentSocketName();
				if (AttachedSocketName == Pair.Key)
				{
					// 销毁 Actor
					ChildActor->Destroy();
				}
			}
		}

		IRL_ItemInstanceHolder::Execute_SetItemInstance(AttachedActor, ItemInstance);

		// 尝试获取目标插槽，附加到物体上
		if (SkeletalMeshComp && !Pair.Key.IsNone())
		{
			// 使用 AttachToComponent 将附加的 Actor 绑定到插槽上
			EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
			EAttachmentRule RotationRule = EAttachmentRule::SnapToTarget;
			EAttachmentRule ScaleRule = EAttachmentRule::KeepRelative;
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(LocationRule, RotationRule, ScaleRule, true);
			AttachedActor->AttachToComponent(SkeletalMeshComp, AttachmentTransformRules, Pair.Key);

			AttachedActor->SetActorRelativeLocation(FVector::ZeroVector);
			AttachedActor->SetActorRelativeRotation(FRotator::ZeroRotator);

			//设置主骨骼
			AArmorBase* Armor = Cast<AArmorBase>(AttachedActor);
			if (Armor)
			{
				Armor->Bone->SetLeaderPoseComponent(SkeletalMeshComp);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SkeletalMeshComponent or socket name not found on the ItemActor."));
			return nullptr;
		}
	}
	return ItemInstance;
}

void URLItemFragment_Attached::RemoveArmor(AActor* Owner_T) const
{
	USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(Owner_T->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	TArray<AActor*> AttachedActors;
	SkeletalMeshComp->GetOwner()->GetAttachedActors(AttachedActors);
	for (auto Pair : AttachedMap)
	{
		for (auto ChildActor : AttachedActors)
		{
			if (ChildActor)
			{
				// 检查该 Actor 是否附加到目标插槽
				FName AttachedSocketName = ChildActor->GetAttachParentSocketName();
				if (AttachedSocketName == Pair.Key)
				{
					// 销毁 Actor
					ChildActor->Destroy();
				}
			}
		}
	}
}

