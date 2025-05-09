// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/RL_EnemyMovementComponent.h"
#include "Components/SplineComponent.h"


URL_EnemyMovementComponent::URL_EnemyMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void URL_EnemyMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner())
	{
		PatrolSpline = GetOwner()->FindComponentByClass<USplineComponent>();
	}
}


void URL_EnemyMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	//初始化路径点数组
	for (int32 i = 0; i < PatrolSpline->GetNumberOfSplinePoints(); i++)
	{
		PatrolPoints.Add(PatrolSpline->GetWorldLocationAtSplinePoint(i));
	}
	
}

