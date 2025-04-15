// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_InputDisableAll.h"
#include <EnhancedInputSubsystems.h>


void UANS_InputDisableAll::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration,EventReference);
	AActor* Owner = MeshComp->GetOwner();

	if (!Owner) return;

	// “∆≥˝ ‰»Î
	if (APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (InputSubsystem->HasMappingContext(DefaultIMC))
			{
				InputSubsystem->RemoveMappingContext(DefaultIMC);
				InputSubsystem->AddMappingContext(DisableIMC, 0);
			}
		}
	}
}
