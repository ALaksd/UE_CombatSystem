// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/GA_Dodge.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Animation/RL_AnimInstanceBase.h"
#include "Component/RL_MovementComponent.h"

UGA_Dodge::UGA_Dodge()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Dodge::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		Super::InputReleased(Handle, ActorInfo, ActivationInfo);

		EDirectionType type = Cast<URL_AnimInstanceBase>(ActorInfo->SkeletalMeshComponent->GetAnimInstance())->DicType; 

		if (ActorInfo->AvatarActor->FindComponentByClass<URL_MovementComponent>()->CurrentMovementState != EMovementState::Running)
		{
			if (RollMontage && !ActorInfo->AvatarActor->Tags.Contains(FName("IsLocking")))
			{
				UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
					this,
					NAME_None,
					RollMontage,
					1.0f,
					NAME_None,
					false
				);

				if (PlayMontageTask)
				{
					PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
					PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
					PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
					PlayMontageTask->ReadyForActivation();
				}
			}
			else if (ActorInfo->AvatarActor->GetComponentByClass<UCharacterMovementComponent>()->Velocity.IsNearlyZero())
			{
				UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
							this,
							NAME_None,
							DodgeMontageB,
						1.0f,
					NAME_None,
					false
							);

				if (PlayMontageTask)
				{
					PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
					PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
					PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
					PlayMontageTask->ReadyForActivation();
				}
			}
			else if (ActorInfo->AvatarActor->Tags.Contains(FName("IsLocking")))
			{
				switch (type)
				{
					case EDirectionType::ForwardDic:
						{
							UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
							this,
							NAME_None,
							DodgeMontageF,
						1.0f,
					NAME_None,
					false
							);

							if (PlayMontageTask)
							{
								PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->ReadyForActivation();
							}
						}
					break;
					case EDirectionType::BackwardDic:
						{
							UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
							this,
							NAME_None,
							DodgeMontageB,
						1.0f,
					NAME_None,
					false
							);

							if (PlayMontageTask)
							{
								PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->ReadyForActivation();
							}
						}
					break;
					case EDirectionType::LeftDic:
						{
							UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
							this,
							NAME_None,
							DodgeMontageL,
						1.0f,
					NAME_None,
					false
							);

							if (PlayMontageTask)
							{
								PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->ReadyForActivation();
							}
						}
					break;
					case EDirectionType::RightDic:
						{
							UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
							this,
							NAME_None,
							DodgeMontageR,
						1.0f,
					NAME_None,
					false
							);

							if (PlayMontageTask)
							{
								PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnMontageCompleted);
								PlayMontageTask->ReadyForActivation();
							}
						}
					break;
				}
				
			}
		}
	}
}

void UGA_Dodge::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

