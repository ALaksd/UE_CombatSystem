// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Data/RL_CharacterData.h"
#include "InputActionValue.h"
#include "RL_BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class URL_PlayerStateWidget;

UCLASS()
class UE_ROUGELIKEDEMO_API ARL_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARL_BaseCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//FORCEINLINE ����ǿ�����������������ڶ�ε��õļ򵥺��������ٺ������ÿ�������ߴ���Ч��
	FORCEINLINE class USpringArmComponent* GetSpringArmComponent() { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetCameraComponent() { return FllowCamera; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FllowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|UI")
	TSubclassOf<UUserWidget> PlayerStateUIClass;

	///*���ù���*/
	//UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> HeadConstant;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Shoulder;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Hair;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Boots;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Chest;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Back;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Hands;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Helmet;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RLCharacter|Components")
	//TObjectPtr<USkeletalMeshComponent> Legs;

	/**
	 * ��ɫ�����Լ��ƶ���ص����ã���Щ������ܻ��Ż���һ���������
	 */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> BaseIMC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	//�ܲ�����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RunAction;

	//��������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RLCharacter|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category = "RLCharacter|Settings")
	//��ɫ�ƶ�״̬����
	TMap<EMovementState, FMovementSetting> MovementSettingMap;

	//��ɫ��ǰ�ƶ�״̬
	UPROPERTY(BlueprintReadWrite)
	EMovementState CurrentMovmentState;
	//���½�ɫ�ƶ�״̬
	UFUNCTION(BlueprintCallable)
	void UpdateMovementState(EMovementState State);
	
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Run(const FInputActionValue& Value);

	void Roll(const FInputActionValue& Value);

private:
	//��ɫ״̬UI
	TObjectPtr<URL_PlayerStateWidget> PlayerStateUI;
};

