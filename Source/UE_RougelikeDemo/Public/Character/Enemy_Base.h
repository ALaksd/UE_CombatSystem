// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Pawn.h"
#include "Enemy_Base.generated.h"

UCLASS()
class UE_ROUGELIKEDEMO_API AEnemy_Base : public APawn
{
	GENERATED_BODY()

public:
	AEnemy_Base();


private:
	//胶囊碰撞体组件
	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<UCapsuleComponent> Capsule;
	//骨骼网格体组件
	UPROPERTY(EditDefaultsOnly,Category="Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	//属性
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeSet> AttributeSet;




public:
	inline UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }

	
protected:
	virtual void BeginPlay() override;

private:
	void InitAbilityActorInfo();
	
};
