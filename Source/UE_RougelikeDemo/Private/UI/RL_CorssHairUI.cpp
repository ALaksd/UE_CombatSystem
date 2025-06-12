// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RL_CorssHairUI.h"

void URL_CorssHairUI::NativeConstruct()
{
	Super::NativeConstruct();
	AActor* Owner = GetOwningPlayerPawn();
	if (Owner)
	{
		URL_RowCompoment* BowComponent = Owner->FindComponentByClass<URL_RowCompoment>();
		if (BowComponent)
		{
			BowComponent->OnPullBegin.AddUObject(this, &URL_CorssHairUI::ShortCrossHair);
			BowComponent->OnPullEnd.AddUObject(this, &URL_CorssHairUI::ResetCrossHair);
		}
	}
}

void URL_CorssHairUI::ShortCrossHair(float Pulltime, float MaxPullTime)
{
	CircleScale = FMath::Clamp(Pulltime / MaxPullTime, 0.0f, 1.0f);
	//UE_LOG(LogTemp, Warning, TEXT("%f"), CircleScale);
	OuterCircle->SetRenderScale(FVector2D(1 - CircleScale, 1 - CircleScale));
	if (CircleScale >= 0.9)
	{
		PlayScaleAnimation();
	}
	else
	{
		ResetScaleAnimation();
	}
}

void URL_CorssHairUI::ResetCrossHair()
{
	OuterCircle->SetRenderScale(FVector2D(1, 1));
}
