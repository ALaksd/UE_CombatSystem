// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RL_MusicSubSystem.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void URL_MusicSubSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// 不在这里创建音频组件
}

void URL_MusicSubSystem::Deinitialize()
{
	// 停止并释放音频组件
	StopBackgroundMusic(0.0f);
	Super::Deinitialize();
}

void URL_MusicSubSystem::PlayBackgroundMusic(USoundBase* Music, float FadeInDuration)
{
	if (!Music) return;

	// 如果已经有音乐在播放，先停止
	if (BackgroundAudioComponent && BackgroundAudioComponent->IsPlaying())
	{
		StopBackgroundMusic(FadeInDuration);
	}

	// 保存当前音乐
	CurrentMusic = Music;

	// 使用正确的2D音效生成方法
	UWorld* World = GetWorld();
	if (!World) return;

	BackgroundAudioComponent = UGameplayStatics::SpawnSound2D(
		World,
		Music,
		1.0f,  // Volume
		1.0f,  // Pitch
		0.0f,  // StartTime
		nullptr, // SoundConcurrency
		true,    // bPersistAcrossLevelTransition
		false    // bAutoDestroy
	);

	// 应用淡入效果
	if (BackgroundAudioComponent && FadeInDuration > 0.0f)
	{
		BackgroundAudioComponent->FadeIn(FadeInDuration);
	}
}

void URL_MusicSubSystem::StopBackgroundMusic(float FadeOutDuration)
{
	if (!BackgroundAudioComponent || !BackgroundAudioComponent->IsPlaying())
		return;

	if (FadeOutDuration > 0.0f)
	{
		// 应用淡出效果
		BackgroundAudioComponent->FadeOut(FadeOutDuration, 0.0f);

		FTimerHandle StopTimer;
		// 设置定时器在淡出完成后销毁组件
		GetWorld()->GetTimerManager().SetTimer(StopTimer,
			[this]()
			{
				if (BackgroundAudioComponent)
				{
					BackgroundAudioComponent->Stop();
					BackgroundAudioComponent->DestroyComponent();
					BackgroundAudioComponent = nullptr;
				}
			},
			FadeOutDuration,
			false
		);
	}
	else
	{
		// 立即停止
		BackgroundAudioComponent->Stop();
		BackgroundAudioComponent->DestroyComponent();
		BackgroundAudioComponent = nullptr;
	}
}

bool URL_MusicSubSystem::IsMusicPlaying() const
{
	return BackgroundAudioComponent && BackgroundAudioComponent->IsPlaying();
}