// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_ExecuteSkill.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "Component/RL_EnemyMovementComponent.h"
#include "AI/RL_AIController.h"

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
				AbilityEndedDelegateHandle = ASC->AbilityEndedCallbacks.AddUObject(this, &UBTTask_ExecuteSkill::OnAbilityEnded);

				// 尝试激活技能
				const bool bSuccess = ASC->TryActivateAbility(Spec.Handle);
				//ASC->TryActivateAbilitiesByTag()
				if (!bSuccess)
				{
					// 技能激活失败
					UE_LOG(LogTemp, Warning, TEXT("ExecuteSkill Failed: Ability activation failed (Tag: %s)"), *SkillTag.ToString());
				}
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
	OwnerComp.GetBlackboardComponent()->SetValueAsName(SelectedSkillKey.SelectedKeyName, FName("None"));


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
