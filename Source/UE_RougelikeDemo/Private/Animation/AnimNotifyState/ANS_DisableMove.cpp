// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_DisableMove.h"
#include <EnhancedInputSubsystems.h>

void UANS_DisableMove::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	AActor* Owner = MeshComp->GetOwner();

	if (!Owner) return;

	// 移除输入
	if (APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			// 单独移除移动操作的MappingContext
			if (InputSubsystem->HasMappingContext(MoveIMC))
			{
				InputSubsystem->RemoveMappingContext(MoveIMC);
			}
		}
	}
}

void UANS_DisableMove::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation,EventReference);
	AActor* Owner = MeshComp->GetOwner();

	if (!Owner) return;

	// 移除输入
	if (APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			InputSubsystem->AddMappingContext(MoveIMC, 1); // 加回移动控制

		}
	}
}
