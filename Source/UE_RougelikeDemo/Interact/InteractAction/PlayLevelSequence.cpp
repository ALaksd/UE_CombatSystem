// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayLevelSequence.h"

#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"

void UPlayLevelSequence::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	if (!SequenceToPlay || !Interactor->GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("未设置LevelSequence或世界无效"));
		return;
	}

	//FMovieSceneSequencePlaybackSettings PlaybackSettings;
	//PlaybackSettings.bAutoPlay = true;
	//
	//ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
	//	Interactor->GetWorld(), SequenceToPlay, PlaybackSettings, OutActor
	//);

	//SequencePlayer->OnFinished.AddDynamic(this, &UPlayLevelSequence::OnSequenceFinished);
	//
	//if (SequencePlayer)
	//{
	//	SequencePlayer->Play();
	//}

	//InteractedTarget->Destroy();
}

// 然后在你的回调中：
void UPlayLevelSequence::OnSequenceFinished()
{
	if (OutActor)
	{
		OutActor->Destroy();
	}
}