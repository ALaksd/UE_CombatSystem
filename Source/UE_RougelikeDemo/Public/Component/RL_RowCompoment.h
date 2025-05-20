// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/RL_WeaponCompoment.h"
#include "Weapon/RL_Bow.h"
#include "RL_RowCompoment.generated.h"

// 声明委托，用于开始和结束瞄准时通知
DECLARE_MULTICAST_DELEGATE(FOnAimBeginDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAimEndDelegate);

//委托，用于拉弓时的通知
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPullBeginDelegate, float MaxPullTime, float CurrentPullTime);
DECLARE_MULTICAST_DELEGATE(FOnPullEndDelegate);

UCLASS()
class UE_ROUGELIKEDEMO_API URL_RowCompoment : public URL_WeaponCompoment
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

	//瞄准开始
	void AimBegin();

	//结束瞄准
	void AimEnd();

	//发射箭时
	void FireArrowBegin();

	//发射箭结束
	void FireArrowEnd();

	//拉弓开始
	void PullBowBegin();

	//拉弓结束
	void PullBowEnd();

	// 瞄准开始和结束的委托
	FOnAimBeginDelegate OnAimBegin;

	FOnAimEndDelegate OnAimEnd;

	FOnPullBeginDelegate OnPullBegin;

	FOnPullEndDelegate OnPullEnd;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Arrow)
	//是否处于瞄准状态
	bool IsAiming;

	//是否正在拉弓
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Arrow)
	bool IsPulling;

	//箭生成的插槽
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Arrow)
	FName ArrowSocket;

	//生成的箭类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Arrow)
	TSubclassOf<class ARL_Arrow> ArrowClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Arrow)
	class UAnimMontage* FireMontage;

	//生成的箭的引用
	class ARL_Arrow* SpawnedArrow;

	//定时器用于储存拉弓的时长
	FTimerHandle PullBowTimeHandle;

	//更新时间轴的秒数(拉弓时长增量)
	float PullIncreasTime = 0.033f;
	
	//拉弓时长
	float PullBowDuration;

protected:
	//蒙太奇播放结束
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//生成箭
	void SpawnArrow();

	//销毁箭
	void DestroyArrow();

	//发射箭
	void FireArrow(float Strength);

	//计算发射箭的方向
	FVector CalculateArrowDirection();

	//定时器更新函数
	void UpdatePullBowDuration();

	//播放动画
	void PlayMontage(class UAnimMontage* Montage);

private:

	FRotator AimRotaion;
	FRotator InitialRotaion;
	ARL_Bow* Bow;
	//是否正在射击
	bool IsFiring;

	//输入缓冲
	bool IsWaitingToPulling;
};
