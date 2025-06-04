// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_Teleport.h"

#include "Component/RL_EnemyMovementComponent.h"
#include "AIController.h"
#include "Animation/AnimationNotify/AN_Teleport.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTTask_Teleport::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	Location=Blackboard->GetValueAsVector(FName("NextToMoveLocation"));
	
	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return EBTNodeResult::Failed;

	URL_EnemyMovementComponent* EnemyMove = ControlledPawn->FindComponentByClass<URL_EnemyMovementComponent>();
	if (!EnemyMove) return EBTNodeResult::Failed;

	const URL_EnemyConfig* EnemyConfig = EnemyMove->GetEnemyConfig();
	if (!EnemyConfig) return EBTNodeResult::Failed;

	for (FEnemySkills EnemySkill : EnemyConfig->EnemySkills)
	{
		if (EnemySkill.AbilityTag == FGameplayTag::RequestGameplayTag(FName("Enemy.Ability.Roll")))
		{
			USkeletalMeshComponent* Mesh = ControlledPawn->FindComponentByClass<USkeletalMeshComponent>();
			if (!Mesh) return EBTNodeResult::Failed;
			
			UAnimInstance* Anim = Mesh->GetAnimInstance();
			if (!Anim) return EBTNodeResult::Failed;

			// 播放瞬移的蒙太奇
			Anim->Montage_Play(EnemySkill.Animations[0].Montage);
 
			for (FAnimNotifyEvent& Notify : EnemySkill.Animations[0].Montage->Notifies)
			{
				UAN_Teleport* Event = Cast<UAN_Teleport>(Notify.Notify);
				if (Event)
				{
					Event->OnNotifyReceived.AddDynamic(this,&UBTTask_Teleport::TeleportTo);
				}
			}
			
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}

void UBTTask_Teleport::TeleportTo(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AActor* OWner = MeshComp->GetOwner())
	{
		OWner->SetActorLocation(Location);
	}
}
