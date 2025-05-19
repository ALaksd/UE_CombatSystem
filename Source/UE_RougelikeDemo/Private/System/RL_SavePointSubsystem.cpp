// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RL_SavePointSubsystem.h"
#include <RL_CharacterSelectionWidget.cpp>
#include "InteractableActor/Interactable_LanternFlame.h"
#include <System/RL_UIManagerSubsystem.h>
#include <Player/RL_PlayerState.h>
#include "GameFramework/Character.h"
#include <System/RL_SanitySubsystem.h>

void URL_SavePointSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &URL_SavePointSubsystem::HandlePostLoadMap);
}

void URL_SavePointSubsystem::RegisterSavePoint(const FSavePointData& SavePointData)
{
	SavePoints.Add(SavePointData.PointID, SavePointData);
}

TArray<FSavePointData> URL_SavePointSubsystem::GetAvailableSavePoints() const
{
	TArray<FSavePointData> Result;
	//从这个map中的值生成一个数组。
	SavePoints.GenerateValueArray(Result);
	return Result;
}

void URL_SavePointSubsystem::TravelToSavePoint(const FName& TargetPointID)
{
	if (!SavePoints.Contains(TargetPointID)) return;

	const FSavePointData& TargetData = SavePoints[TargetPointID];
	UWorld* World = GetWorld();

	if (TargetData.MapName == World->GetMapName())
	{
		// 同一地图直接传送
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0))
		{
			PlayerPawn->SetActorLocationAndRotation(
				TargetData.Location,
				TargetData.Rotation
			);
		}
	}
	else
	{
		// 跨地图传送
		PendingTravelPointID = TargetPointID;
		FString Options = FString::Printf(TEXT("?SavePointID=%s"), *TargetPointID.ToString());
		UGameplayStatics::OpenLevel(World, TargetData.MapName, true, Options);
	}
	
	Reset(TargetData);
}

void URL_SavePointSubsystem::TravelToCurrentPoint()
{
	TravelToSavePoint(CurrentSavePointID);
}

void URL_SavePointSubsystem::Reset(const FSavePointData& SavePointData)
{
	//重置生成敌人
	ResetEnemy(SavePointData);

	//重置属性
	ResetAttributes();	

	UGameInstance* GameInstance = GetGameInstance();
	
	//重置理智
	URL_SanitySubsystem* SanitySubsystem = GameInstance->GetSubsystem<URL_SanitySubsystem>();
	if (SanitySubsystem)
	{
		SanitySubsystem->RestoreSanityToMax();
	}

	//重置UI
	URL_UIManagerSubsystem* UIManagerSubsystem = GameInstance->GetSubsystem<URL_UIManagerSubsystem>();
	if (UIManagerSubsystem)
	{
		UIManagerSubsystem->ResetUI();
	}
	//重置血瓶数量，调用仓库System的函数
}

void URL_SavePointSubsystem::ResetEnemy(const FSavePointData& SavePointData)
{
	if (SavePointData.LanternFlamePtr)
	{
		SavePointData.LanternFlamePtr->ResetEnemySpawn();
	}
}


void URL_SavePointSubsystem::ResetAttributes()
{
	ARL_PlayerState* PlayerState = Cast<ARL_PlayerState>(UGameplayStatics::GetPlayerCharacter(this, 0)->GetPlayerState());
	if (PlayerState)
	{
		PlayerState->ResetAttribute();
	}

}

void URL_SavePointSubsystem::HandlePostLoadMap(UWorld* LoadedWorld)
{
	if (!PendingTravelPointID.IsNone() && SavePoints.Contains(PendingTravelPointID))
	{
		const FSavePointData& TargetData = SavePoints[PendingTravelPointID];
		if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
		{
			PlayerPawn->SetActorLocationAndRotation(
				TargetData.Location,
				TargetData.Rotation
			);
		}
		PendingTravelPointID = NAME_None;
	}
}
