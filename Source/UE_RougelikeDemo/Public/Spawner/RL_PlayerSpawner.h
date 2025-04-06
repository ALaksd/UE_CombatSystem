// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RL_CharacterEunm.h"
#include "RL_PlayerSpawner.generated.h"

UCLASS()
class UE_ROUGELIKEDEMO_API ARL_PlayerSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARL_PlayerSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//��ѡ��Ľ�ɫ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = Character)
	class USkeletalMeshComponent* SwordMan;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	class USkeletalMeshComponent* Archer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	class USkeletalMeshComponent* Caster;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = UI)
	TSubclassOf<UUserWidget> CharacterSelectionWidgetClass;

public:	
	//��ɫѡ��
	void SelectCharacter(int Index);

protected:
	class URL_CharacterSelectionWidget* CharacterSelectionWidget;

	ECharacterType SelectedCharacterType;  // �洢ѡ��Ľ�ɫ����
};
