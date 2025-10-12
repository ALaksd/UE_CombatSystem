// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RL_MusicSubSystem.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_MusicSubSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// 播放背景音乐
	UFUNCTION(BlueprintCallable, Category = "Music")
	void PlayBackgroundMusic(USoundBase* Music, float FadeInDuration = 1.0f);

	// 停止背景音乐
	UFUNCTION(BlueprintCallable, Category = "Music")
	void StopBackgroundMusic(float FadeOutDuration = 1.0f);

	// 检查音乐是否正在播放
	UFUNCTION(BlueprintCallable, Category = "Music")
	bool IsMusicPlaying() const;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY(Transient)
	UAudioComponent* BackgroundAudioComponent = nullptr;

	// 当前播放的声音资源
	UPROPERTY(Transient)
	USoundBase* CurrentMusic = nullptr;
};
