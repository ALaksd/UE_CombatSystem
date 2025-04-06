// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/RL_WeaponCompoment.h"
#include "Weapon/RL_Bow.h"
#include "RL_RowCompoment.generated.h"

// ����ί�У����ڿ�ʼ�ͽ�����׼ʱ֪ͨ
DECLARE_MULTICAST_DELEGATE(FOnAimBeginDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAimEndDelegate);

//ί�У���������ʱ��֪ͨ
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPullBeginDelegate, float MaxPullTime, float CurrentPullTime);
DECLARE_MULTICAST_DELEGATE(FOnPullEndDelegate);

UCLASS()
class UE_ROUGELIKEDEMO_API URL_RowCompoment : public URL_WeaponCompoment
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

	//��׼��ʼ
	void AimBegin();

	//������׼
	void AimEnd();

	//�����ʱ
	void FireArrowBegin();

	//���������
	void FireArrowEnd();

	//������ʼ
	void PullBowBegin();

	//��������
	void PullBowEnd();

	// ��׼��ʼ�ͽ�����ί��
	FOnAimBeginDelegate OnAimBegin;

	FOnAimEndDelegate OnAimEnd;

	FOnPullBeginDelegate OnPullBegin;

	FOnPullEndDelegate OnPullEnd;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Arrow)
	//�Ƿ�����׼״̬
	bool IsAiming;

	//�Ƿ���������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Arrow)
	bool IsPulling;

	//�����ɵĲ��
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Arrow)
	FName ArrowSocket;

	//���ɵļ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Arrow)
	TSubclassOf<class ARL_Arrow> ArrowClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = Arrow)
	class UAnimMontage* FireMontage;

	//���ɵļ�������
	class ARL_Arrow* SpawnedArrow;

	//��ʱ�����ڴ���������ʱ��
	FTimerHandle PullBowTimeHandle;

	//����ʱ���������(����ʱ������)
	float PullIncreasTime = 0.033f;
	
	//����ʱ��
	float PullBowDuration;

protected:
	//��̫�沥�Ž���
	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	//���ɼ�
	void SpawnArrow();

	//���ټ�
	void DestroyArrow();

	//�����
	void FireArrow(float Strength);

	//���㷢����ķ���
	FVector CalculateArrowDirection();

	//��ʱ�����º���
	void UpdatePullBowDuration();

	//���Ŷ���
	void PlayMontage(class UAnimMontage* Montage);

private:

	FRotator AimRotaion;
	FRotator InitialRotaion;
	ARL_Bow* Bow;
	//�Ƿ��������
	bool IsFiring;

	//���뻺��
	bool IsWaitingToPulling;
};
