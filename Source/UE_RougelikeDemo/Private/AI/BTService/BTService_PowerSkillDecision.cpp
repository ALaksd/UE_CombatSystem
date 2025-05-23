// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_PowerSkillDecision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

void UBTService_PowerSkillDecision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return ;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;

	// 获取 EnemyMovementComponent
	URL_EnemyMovementComponent* EnemyMove = ControlledPawn->FindComponentByClass<URL_EnemyMovementComponent>();
	if (!EnemyMove) return ;

	// 获取配置的 EnemyDataAsset
	const URL_EnemyConfig* EnemyConfig = EnemyMove->GetEnemyConfig();
	if (!EnemyConfig) return ;

	// 获取技能列表
	const TArray<FEnemySkills>& WakingSkills = EnemyConfig->EnemySkills;

	/**
	 * 收集可用技能
	 * 1.技能冷却结束
	 * 2.到达攻击范围
	 */
	TArray<FEnemySkills> PowerfulSkills;
	for (const FEnemySkills& Skill : WakingSkills)
	{
		if (CheckSkillCondition(Skill, AIController))
		{
			PowerfulSkills.Add(Skill);
		}
	}

	if (PowerfulSkills.Num() > 0)
	{
		PowerfulSkills.Sort([](const FEnemySkills& A, const FEnemySkills& B) {
			return A.PriorityLevel > B.PriorityLevel;
			});

		// 如果有可用技能，选择第一个（优先级最高的）
		const FEnemySkills& SelectedSkill = PowerfulSkills[0];
		FString String = *SelectedSkill.AbilityTag.ToString();
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, String);
		OwnerComp.GetBlackboardComponent()->SetValueAsName(SelectedPowerSkillKey.SelectedKeyName, SelectedSkill.AbilityTag.GetTagName());
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsName(SelectedPowerSkillKey.SelectedKeyName, FName("None"));
	}

}

bool UBTService_PowerSkillDecision::CheckSkillCondition(const FEnemySkills& Skill, AAIController* AIController)
{
	bool bPower = Skill.bIsPowerfulAttack;
	bool bNotCoolDown = !GetAbilitySystem(AIController->GetPawn())->HasMatchingGameplayTag(
		FGameplayTag::RequestGameplayTag(FName(FString::Printf(TEXT("Cooldown.%s"), *Skill.AbilityTag.ToString())))
	);
	return bPower && bNotCoolDown;
}

UAbilitySystemComponent* UBTService_PowerSkillDecision::GetAbilitySystem(APawn* Pawn) const
{
	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		return ASC;
	}
	return nullptr;
}