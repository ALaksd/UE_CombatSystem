// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_AttackTest.generated.h"

class ARL_BaseCharacter;
/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API UANS_AttackTest : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	//调用通知的Character
	UPROPERTY()
	TObjectPtr<ACharacter> Caller;

	
public:
	//伤害Effect
	UPROPERTY(EditAnywhere,Category = "Attribute")
	TSubclassOf<UGameplayEffect> DamageSpecHandle;
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;
	
	
};
