// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Prop/GA_HealthBottle.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Component/RL_MovementComponent.h"
#include "UE_RougelikeDemo/InventorySystem/InventoryComponent/RLInventoryComponent_Equipment.h"
#include "UE_RougelikeDemo\InventorySystem\RLInventoryItemInstance.h"

UGA_HealthBottle::UGA_HealthBottle()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_HealthBottle::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	int32 BottleCount;
	URLInventoryComponent_Equipment* InventoryComp = ActorInfo->OwnerActor.Get()->FindComponentByClass<URLInventoryComponent_Equipment>();
	if (InventoryComp)
	{
		FRLInventoryItemSlotHandle InventorySlotHandle = InventoryComp->GetSlotHandleByTag(UseItemTag);
		URLInventoryItemInstance* ItemInstance = InventoryComp->GetItemInstanceInSlot(InventorySlotHandle);
	
		if (ItemInstance)
		{
			BottleCount = ItemInstance->GetCurrentStack();

			//物品数量大于0
			if (BottleCount > 0)
			{
				ActorInfo->AvatarActor->FindComponentByClass<URL_MovementComponent>()->UpdateMovementState(EMovementState::Walking);
				if (UseItemMontage)
				{
					UAbilityTask_PlayMontageAndWait* PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
						this,
						NAME_None,
						UseItemMontage,
						1.0f,
						NAME_None,
						false
					);

					if (PlayMontageTask)
					{
						PlayMontageTask->OnCompleted.AddDynamic(this, &UGA_HealthBottle::OnMontageCompleted);
						PlayMontageTask->OnCancelled.AddDynamic(this, &UGA_HealthBottle::OnMontageCompleted);
						PlayMontageTask->OnInterrupted.AddDynamic(this, &UGA_HealthBottle::OnMontageCompleted);
						PlayMontageTask->ReadyForActivation();
					}

					//数量减1
					ItemInstance->AddStack(-1);
					InventoryComp->OnItemSlotUpdate.Broadcast(InventoryComp, InventorySlotHandle, ItemInstance, ItemInstance);
				}
			}
		}
		
	}

}

void UGA_HealthBottle::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	ActorInfo->AvatarActor->FindComponentByClass<URL_MovementComponent>()->UpdateMovementState(EMovementState::Jogging);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_HealthBottle::OnMontageCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
