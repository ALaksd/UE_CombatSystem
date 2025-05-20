// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_InputDisableAll.h"
#include <EnhancedInputSubsystems.h>


void UANS_InputDisableAll::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration,EventReference);
	AActor* Owner = MeshComp->GetOwner();

	if (!Owner) return;

	// 移除输入
 	if (APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (InputSubsystem->HasMappingContext(DefaultIMC) && InputSubsystem->HasMappingContext(MoveIMC))
			{
				InputSubsystem->RemoveMappingContext(DefaultIMC);
				InputSubsystem->RemoveMappingContext(MoveIMC);
				InputSubsystem->AddMappingContext(DisableIMC, 0);
			}
		}
	}
}
