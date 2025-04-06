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
	//ʱ�������
	UPROPERTY()
	UTimelineComponent* AimTimeline;

	//ʱ��������
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* AimCurve;

	//ʱ����ص�����
	UFUNCTION()
	void OnAimTimelineUpdate(float Value);

protected:
	//������׼״̬�����
	void SetAimCamera();

	//�ָ������
	void ResetAimCamera();

	//���
	void Fire();

private:
	//��׼�������Ұ
	float AimFov;
	//��ʼ�������Ұ
	float InitialFov;
	//��׼�����ƫ��
	FVector AimCameraOffest;
	//��ʼ�����ƫ��
	FVector InitialCameraOffest;
};
