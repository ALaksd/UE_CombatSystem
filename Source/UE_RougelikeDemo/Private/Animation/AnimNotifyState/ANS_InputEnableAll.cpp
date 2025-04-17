// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/ANS_InputEnableAll.h"
#include <EnhancedInputSubsystems.h>

void UANS_InputEnableAll::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	AActor* Owner = MeshComp->GetOwner();

	if (!Owner) return;

	// ∆Ù∂Ø“∆∂Ø ‰»Î
	if (APlayerController* PC = Cast<APlayerController>(Owner->GetInstigatorController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (InputSubsystem->HasMappingContext(DisableIMC))
			{
				InputSubsystem->RemoveMappingContext(DisableIMC);
				InputSubsystem->AddMappingContext(DefaultIMC, 0);
			}
		}
	}
}
