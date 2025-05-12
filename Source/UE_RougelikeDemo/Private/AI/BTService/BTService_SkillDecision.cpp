// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_SkillDecision.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

UBTService_SkillDecision::UBTService_SkillDecision()
{
	NodeName = "Waking State AttackDecision";
}

void UBTService_SkillDecision::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;

	// 获取 EnemyMovementComponent
	URL_EnemyMovementComponent* EnemyMove = ControlledPawn->FindComponentByClass<URL_EnemyMovementComponent>();
	if (!EnemyMove) return;

	// 获取配置的 EnemyDataAsset
	const URL_EnemyConfig* EnemyConfig = EnemyMove->GetEnemyConfig();
	if (!EnemyConfig) return;

	// 获取技能列表
	const TArray<FEnemySkills>& WakingSkills = EnemyConfig->EnemySkills;

	AvailableSkills.Empty();

	// 遍历所有技能，检查条件并添加到可用列表
	for (const FEnemySkills& Skill : WakingSkills)
	{
		if (CheckSkillCondition(Skill, AIController))
		{
			AvailableSkills.Add(Skill);
		}
	}

	// 根据优先级排序
	AvailableSkills.Sort([](const FEnemySkills& A, const FEnemySkills& B) {
		return A.PriorityLevel > B.PriorityLevel;
		});

	// 如果有可用技能，选择第一个（优先级最高的）
	if (AvailableSkills.Num() > 0)
	{
		const FEnemySkills& SelectedSkill = AvailableSkills[0];
		FString String = *SelectedSkill.AbilityTag.ToString();
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, String);
		OwnerComp.GetBlackboardComponent()->SetValueAsName(SelectedSkillKey.SelectedKeyName, SelectedSkill.AbilityTag.GetTagName());
	}
	else
	{
		// 如果没有可用技能，设置为 None
		OwnerComp.GetBlackboardComponent()->SetValueAsName(SelectedSkillKey.SelectedKeyName, "None");
	}
}


void UBTService_SkillDecision::EvaluateSkills(AAIController* AIController, APawn* ControlledPawn)
{
	//可以添加评估逻辑
}

bool UBTService_SkillDecision::CheckSkillCondition(const FEnemySkills& Skill, AAIController* AIController)
{
	APawn* Pawn = AIController->GetPawn();
	UAbilitySystemComponent* ASC = GetAbilitySystem(Pawn);
	if (!ASC) return false;

	// 构造冷却用的 Tag，用 "Cooldown." 前缀 + 能力名字
	const FGameplayTag CooldownTag = FGameplayTag::RequestGameplayTag(
		FName(FString::Printf(TEXT("Cooldown.%s"), *Skill.AbilityTag.ToString()))
	);

	// 检查是否在冷却中
	if (ASC->HasMatchingGameplayTag(CooldownTag))
	{
		// 技能正在冷却，不能选
		return false;
	}

	//可添加新的条件
	return true;
}

UAbilitySystemComponent* UBTService_SkillDecision::GetAbilitySystem(APawn* Pawn) const
{
	UAbilitySystemComponent* ASC = Pawn->FindComponentByClass<UAbilitySystemComponent>();
	if (ASC)
	{
		return ASC;
	}
	return nullptr;
}

