// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_ExecuteSkill.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "AIController.h"

UBTTask_ExecuteSkill::UBTTask_ExecuteSkill()
{
	NodeName = TEXT("ExecuteSkill");
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_ExecuteSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CachedOwnerComp = &OwnerComp;

	const FName SkillTagName = OwnerComp.GetBlackboardComponent()->GetValueAsName(SelectedSkillKey.SelectedKeyName);
	if(SkillTagName == "None")
		return EBTNodeResult::Failed;


	const FGameplayTag SkillTag = FGameplayTag::RequestGameplayTag(SkillTagName);

	if (UAbilitySystemComponent* ASC = GetAbilitySystem(OwnerComp.GetAIOwner()->GetPawn()))
	{
		// 修正问题3：替换FindAbilitySpecFromTag的实现
		TArray<FGameplayAbilitySpec> ActivatableAbilities = ASC->GetActivatableAbilities();
		for (const FGameplayAbilitySpec& Spec : ActivatableAbilities)
		{
			if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(SkillTag))
			{
				// 修正问题1：绑定正确的委托类型
				AbilityEndedDelegateHandle = ASC->AbilityEndedCallbacks.AddUObject(this, &UBTTask_ExecuteSkill::OnAbilityEnded);

				const bool bSuccess = ASC->TryActivateAbility(Spec.Handle);
				return bSuccess ? EBTNodeResult::InProgress : EBTNodeResult::Failed;
			}
		}
	}
	return EBTNodeResult::Failed;
}


void UBTTask_ExecuteSkill::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// 清理资源
	CachedOwnerComp = nullptr;
	AbilityEndedDelegateHandle.Reset();
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}


void UBTTask_ExecuteSkill::OnAbilityEnded(UGameplayAbility* EndedAbility)
{
	if (CachedOwnerComp)
	{
		const bool bSuccess = (EndedAbility && EndedAbility->GetInstancingPolicy() != EGameplayAbilityInstancingPolicy::NonInstanced);

		// 需要先解绑委托
		if (UAbilitySystemComponent* ASC = EndedAbility->GetAbilitySystemComponentFromActorInfo())
		{
			ASC->AbilityEndedCallbacks.Remove(AbilityEndedDelegateHandle);
		}

		FinishLatentTask(*CachedOwnerComp, bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed);
	}
}

UAbilitySystemComponent* UBTTask_ExecuteSkill::GetAbilitySystem(APawn* Pawn) const
{
	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		return ASC;
	}
	return nullptr;
}
