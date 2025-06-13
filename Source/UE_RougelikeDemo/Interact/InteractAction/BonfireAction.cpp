// Fill out your copyright notice in the Description page of Project Settings.


#include "BonfireAction.h"

#include "Controller/RL_BasePlayerController.h"
#include "InteractableActor/Interactable_LanternFlame.h"
#include "Kismet/GameplayStatics.h"
#include "System/RL_SanitySubsystem.h"
#include "System/RL_UIManagerSubsystem.h"
#include "UI/RL_HUD.h"
#include "UI/WidgetController/RL_LanternFlameController.h"

void UBonfireAction::Execute(AActor* Interactor, AActor* InteractedTarget)
{
	AInteractable_LanternFlame* LanternFlame = Cast<AInteractable_LanternFlame>(InteractedTarget);
	//如果已经激活则显示UI
	if (LanternFlame->bIsActive)
	{
		if (ARL_HUD* RLHUD = Cast<ARL_HUD>(LanternFlame->PlayerController->GetHUD()))
		{
			if (URL_LanternFlameController* LanternFlameWidgetController = RLHUD->GetLanternFlameWidgetController())
			{
				// 初始化数据
				LanternFlameWidgetController->Initialize(LanternFlame->SkillList);

				UGameInstance* GameInstance = InteractedTarget->GetWorld()->GetGameInstance();
				if (GameInstance)
				{
					if (URL_UIManagerSubsystem* UIManager = GameInstance->GetSubsystem<URL_UIManagerSubsystem>())
					{
						LanternFlame->WBP_SavePoint = UIManager->AddNewWidget(LanternFlame->WBP_SavePointClass, UGameplayStatics::GetPlayerController(InteractedTarget, 0));
					}
				}

				// 初始化UI
				LanternFlame->WBP_SavePoint->SetWidgetController(LanternFlameWidgetController);
				LanternFlameWidgetController->BroadcastInitialValue();
				LanternFlame->InitPointName();

				
				// 回复理智
				if (URL_SanitySubsystem* SanitySubsystem = GameInstance->GetSubsystem<URL_SanitySubsystem>())
					SanitySubsystem->RestoreSanityToMax();

				//设置传送点
				if (URL_SavePointSubsystem* SavePointSubsystem = GameInstance->GetSubsystem<URL_SavePointSubsystem>())
				{
					SavePointSubsystem->SetCurrentSavaPoint(InteractedTarget->GetFName());
				}

			}
		}
	}
	else //没有激活则激活
	{
		LanternFlame->ActivatePoint();
	}
}
