// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_OtherAbility.generated.h"

/**
 *  负责处理通用技能的动画通知
 *
 *	播放此动画的Ability将监听此次通知以此决定应用什么Effect
 *  
 */

enum class E_SkillType : uint8;

//通知
DECLARE_MULTICAST_DELEGATE_OneParam(FOnSkill,USkeletalMeshComponent*);


UCLASS()
class UE_ROUGELIKEDEMO_API UAN_OtherAbility : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Data")
	E_SkillType SkillType;

	// 触发的通知
	FOnSkill OnSkill;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
