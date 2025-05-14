// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_EnemyAbility_MeeleAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include <Component/RL_EnemyMovementComponent.h>
#include <System/RL_SanitySubsystem.h>

	void UGA_EnemyAbility_MeeleAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
	{
		if (CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
			URL_EnemyMovementComponent* EnemyMove = ActorInfo->AvatarActor->FindComponentByClass<URL_EnemyMovementComponent>();
			UAnimMontage* MontageToPlay = nullptr;

			if (EnemyMove)
			{
				TArray<FEnemySkills> EnemySkills = EnemyMove->GetEnemyConfig()->EnemySkills;
				for (const auto& EnemySkill : EnemySkills)
				{
					if (AbilityTags.HasTagExact(EnemySkill.AbilityTag))
					{
						MontageToPlay = SelectRandomAnimation(EnemySkill);
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

				if (Task)
				{
					Task->OnCompleted.AddDynamic(this, &UGA_EnemyAbility_MeeleAttack::OnMontageCompleted);
					Task->OnCancelled.AddDynamic(this, &UGA_EnemyAbility_MeeleAttack::OnMontageCancelled);
					Task->OnInterrupted.AddDynamic(this, &UGA_EnemyAbility_MeeleAttack::OnMontageCancelled);
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

	UAnimMontage* UGA_EnemyAbility_MeeleAttack::SelectRandomAnimation(const FEnemySkills& Skills)
	{
		URL_SanitySubsystem* SanitySubsystem = UGameInstance::GetSubsystem<URL_SanitySubsystem>(GetWorld()->GetGameInstance());
		E_SanityState CurrentSanityState = E_SanityState::Sane; // 默认状态

		if (SanitySubsystem)
		{
			CurrentSanityState = SanitySubsystem->GetSanityState();
		}

		// 将理智状态转换为对应的GameplayTag
		FGameplayTag StateTag;
		if (CurrentSanityState == E_SanityState::Sane)
		{
			StateTag = FGameplayTag::RequestGameplayTag("Sanity.State.Sane");
		}
		else 
		{
			StateTag = FGameplayTag::RequestGameplayTag("Sanity.State.Chaotic");
		}

		const TArray<FEnemySkillAnimation>&Anims = Skills.Animations;
		if (Anims.Num() == 0) return nullptr;

		// Step 1: 筛选符合当前状态的动画
		TArray<FEnemySkillAnimation> ValidAnims;
		for (const auto& Anim : Anims)
		{
			// 允许的状态为空表示全部允许
			if (Anim.AllowedStates.IsEmpty() || Anim.AllowedStates.HasTagExact(StateTag))
			{
				ValidAnims.Add(Anim);
			}
		}

		if (ValidAnims.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No valid animations for current sanity state!"));
			return nullptr;
		}

		// Step 2: 计算有效动画的总权重
		float TotalWeight = 0.0f;
		for (const auto& Anim : ValidAnims)
		{
			TotalWeight += Anim.Weight;
		}

		// Step 3: 执行加权随机选择
		float RandomPoint = FMath::FRandRange(0.f, TotalWeight);
		for (const auto& Anim : ValidAnims)
		{
			if (RandomPoint <= Anim.Weight)
			{
				return Anim.Montage;
			}
			RandomPoint -= Anim.Weight;
		}

		return ValidAnims.Last().Montage; // fallback
	}

	void UGA_EnemyAbility_MeeleAttack::OnMontageCompleted()
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}

	void UGA_EnemyAbility_MeeleAttack::OnMontageCancelled()
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
