// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_EnemyAbility_MeeleAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include <Interface/RL_CombatInterface.h>
#include <Interface/RL_EnemyInterface.h>
#include <Component/RL_EnemyMovementComponent.h>

	void UGA_EnemyAbility_MeeleAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
		if (CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			URL_EnemyMovementComponent* EnemyMove = ActorInfo->AvatarActor->FindComponentByClass<URL_EnemyMovementComponent>();
			UAnimMontage* MontageToPlay = nullptr;
			bool bIsRed = false;

			if (EnemyMove)
			{
				TArray<FEnemySkills> EnemySkills = EnemyMove->GetEnemyConfig()->EnemySkills;
				for (const auto& EnemySkill : EnemySkills)
				{
					if (AbilityTags.HasTagExact(EnemySkill.AbilityTag))
					{
						MontageToPlay = SelectRandomAnimation(EnemySkill, bIsRed);
						break;
					}
				}
				
			}

			if (MontageToPlay)
			{
				UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
					this,
					NAME_None,
					MontageToPlay,
					1.0f,
					NAME_None,
					false
				);

				if (bIsRed)
				{
					//红光攻击，比如播放一个GagmeplayCue
				}

				if (Task)
				{
					Task->OnCompleted.AddDynamic(this, &UGA_EnemyAbility_MeeleAttack::OnMontageCompleted);
					Task->OnCancelled.AddDynamic(this, &UGA_EnemyAbility_MeeleAttack::OnMontageCompleted);
					Task->OnBlendOut.AddDynamic(this, &UGA_EnemyAbility_MeeleAttack::OnMontageCompleted);
					Task->OnInterrupted.AddDynamic(this, &UGA_EnemyAbility_MeeleAttack::OnMontageCompleted);
					Task->ReadyForActivation();
				}
			}

		}
	}

	void UGA_EnemyAbility_MeeleAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
	{
		GetWorld()->GetTimerManager().ClearTimer(FacingUpdateTimerHandle);

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	}

	void UGA_EnemyAbility_MeeleAttack::UpdateFacingDirection()
	{
		AActor* SourceActor = GetAvatarActorFromActorInfo();
		//获取攻击目标,并将朝向转向攻击目标
		if (SourceActor->Implements<URL_CombatInterface>())
		{
			if (SourceActor->Implements<URL_EnemyInterface>())
			{
				FVector TargerLocation = IRL_EnemyInterface::Execute_GetCombatTarget(SourceActor)->GetActorLocation();
				IRL_CombatInterface::Execute_SetFacingTarget(SourceActor, TargerLocation);
			}
		}
	}


	UAnimMontage* UGA_EnemyAbility_MeeleAttack::SelectRandomAnimation(const FEnemySkills& Skills, bool& bIsRed)
	{
		const TArray<FEnemySkillAnimation>& Anims = Skills.Animations;
		if (Anims.Num() == 0) return nullptr;

		// 计算总权重
		float TotalWeight = 0.0f;
		for (const auto& Anim : Anims)
		{
			TotalWeight += Anim.Weight;
		}

		float RandomPoint = FMath::FRandRange(0.f, TotalWeight);
		for (const auto& Anim : Anims)
		{
			if (RandomPoint <= Anim.Weight)
			{
				bIsRed = Anim.bRed;
				return Anim.Montage;
			}
			RandomPoint -= Anim.Weight;
		}

		bIsRed = Anims.Last().bRed;
		return Anims.Last().Montage; // fallback
	}
