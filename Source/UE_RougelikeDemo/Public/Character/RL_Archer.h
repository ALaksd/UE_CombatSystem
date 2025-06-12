// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RL_BaseCharacter.h"
#include "Components/TimelineComponent.h"
#include "RL_Archer.generated.h"

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API ARL_Archer : public ARL_BaseCharacter
{
	GENERATED_BODY()
	
public:
	ARL_Archer();

	virtual void BeginPlay() override;

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Component)
	class URL_RowCompoment* BowComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;
	//时间轴组件
	UPROPERTY()
	UTimelineComponent* AimTimeline;

	//时间轴曲线
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* AimCurve;

	//时间轴回调函数
	UFUNCTION()
	void OnAimTimelineUpdate(float Value);

protected:
	//设置瞄准状态摄像机
	void SetAimCamera();

	//恢复摄像机
	void ResetAimCamera();

	//射箭
	void Fire();

private:
	//瞄准摄像机视野
	float AimFov;
	//初始摄像机视野
	float InitialFov;
	//瞄准摄像机偏移
	FVector AimCameraOffest;
	//初始摄像机偏移
	FVector InitialCameraOffest;
};
