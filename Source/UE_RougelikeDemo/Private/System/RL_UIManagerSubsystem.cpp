// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RL_UIManagerSubsystem.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include <Input/RLInputComponent.h>
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/AssetManager.h"

void URL_UIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	//文件路径
	FSoftObjectPath BackActionPath(TEXT("/Game/RLDemo/Input/Actions/UI/IA_UI_BackAction.IA_UI_BackAction"));
	FSoftObjectPath UIContextPath(TEXT("/Game/RLDemo/Input/IMC_UI.IMC_UI"));
	FSoftObjectPath MoveContextPath(TEXT("/Game/RLDemo/Input/IMC_Move.IMC_Move"));
	FSoftObjectPath DefaultContextPath(TEXT("/Game/RLDemo/Input/IMC_Default.IMC_Default"));

	// 创建 StreamableManager
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

		// 异步加载多个资源
	TArray<FSoftObjectPath> AssetsToLoad = { BackActionPath, UIContextPath ,MoveContextPath,DefaultContextPath};
	Streamable.RequestAsyncLoad(
		AssetsToLoad,
		[this, BackActionPath, UIContextPath, MoveContextPath, DefaultContextPath]()
		{
			UInputAction* LoadedBackAction = Cast<UInputAction>(BackActionPath.ResolveObject());
			UInputMappingContext* LoadedUIContext = Cast<UInputMappingContext>(UIContextPath.ResolveObject());
			UInputMappingContext* LoadedDefaultContext = Cast<UInputMappingContext>(DefaultContextPath.ResolveObject());
			UInputMappingContext* LoadedMoveContext = Cast<UInputMappingContext>(MoveContextPath.ResolveObject());

			if (LoadedBackAction && LoadedUIContext && LoadedDefaultContext&&LoadedMoveContext)
			{
				BackAction = LoadedBackAction;
				UIContext = LoadedUIContext;
				DefaultContext = LoadedDefaultContext;
				MoveContext = LoadedMoveContext;
			}
		}
	);
}

URL_UserWidget* URL_UIManagerSubsystem::AddNewWidget(TSubclassOf<URL_UserWidget> WidgetClass, APlayerController* PlayerController, const FWidgetInitParams InitParams)
{
	return PushWidget(WidgetClass, PlayerController, InitParams);
}

void URL_UIManagerSubsystem::ToggleWidget(TSubclassOf<URL_UserWidget> WidgetClass, APlayerController* PlayerController)
{
	// 检查是否已存在
	bool bFound = false;
	for (URL_UserWidget* ExistingWidget : WidgetStack)
	{
		if (ExistingWidget && ExistingWidget->GetClass() == WidgetClass)
		{
			PopWidget(PlayerController); // 关闭已存在的
			bFound = true;
			break;
		}
	}

	if (!bFound)
	{
		PushWidget(WidgetClass, PlayerController); // 打开新的
	}
}

URL_UserWidget* URL_UIManagerSubsystem::PushWidget(TSubclassOf<URL_UserWidget> WidgetClass, APlayerController* PlayerController, const FWidgetInitParams& InitParams)
{
	if (!WidgetClass || !PlayerController) return nullptr;

	// 创建Widget并添加到视口
	URL_UserWidget* NewWidget = CreateWidget<URL_UserWidget>(PlayerController, WidgetClass);
	if (NewWidget)
	{
		// 参数初始化（通过接口传递）
		NewWidget->ApplyInitParams(InitParams);

		const int32 ZOrder = WidgetStack.Num();
		NewWidget->AddToViewport(ZOrder);
		WidgetStack.Add(NewWidget);

		// 绑定返回键输入（首次压入时绑定）
		if (WidgetStack.Num() == 1)
		{
			if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				// 加载配置的资源（确保已设置UIContext）
				if (const UInputMappingContext* LoadedUIContext = UIContext.LoadSynchronous())
				{
					InputSubsystem->AddMappingContext(LoadedUIContext,1);
				}
				if (const UInputMappingContext* LoadedDefaultContext = DefaultContext.LoadSynchronous())
				{
					InputSubsystem->RemoveMappingContext(LoadedDefaultContext);
				}
				if (const UInputMappingContext* LoadedMoveContext = MoveContext.LoadSynchronous())
				{
					InputSubsystem->RemoveMappingContext(LoadedMoveContext);

				}
			}

			//设置全局时间膨胀
			UGameplayStatics::SetGlobalTimeDilation(this, 0.f);
			
			// 绑定返回键（使用异步加载确保资源可用）
			BackAction.LoadSynchronous();
			BindBackInput(PlayerController);
		}

		// 设置输入模式为UI
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(NewWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);

		return NewWidget;
	}
	return nullptr;
}

void URL_UIManagerSubsystem::PopWidget(APlayerController* PlayerController)
{
	if (WidgetStack.Num() == 0 || !PlayerController) return;

	// 移除栈顶Widget
	URL_UserWidget* TopWidget = WidgetStack.Last();
	TopWidget->RemoveFromParent();
	WidgetStack.Pop();

	// 更新输入模式
	if (WidgetStack.Num() > 0)
	{
		URL_UserWidget* NextWidget = WidgetStack.Last();
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(NextWidget->TakeWidget());
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(true);

	}
	else
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (const UInputMappingContext* LoadedContext = UIContext.LoadSynchronous())
			{
				InputSubsystem->RemoveMappingContext(LoadedContext);
			}

			if (const UInputMappingContext* LoadedDefaultContext = DefaultContext.LoadSynchronous())
			{
				InputSubsystem->AddMappingContext(LoadedDefaultContext,0);
			}

			if (const UInputMappingContext* LoadedMoveContext = MoveContext.LoadSynchronous())
			{
				InputSubsystem->AddMappingContext(LoadedMoveContext,0);

			}
		}
		//恢复时间膨胀
		UGameplayStatics::SetGlobalTimeDilation(this, 1.f);

		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);
	}
}

void URL_UIManagerSubsystem::BindBackInput(APlayerController* PlayerController)
{
	if (!PlayerController || InputHandles.Contains(PlayerController)) return;

	if (URLInputComponent* RLInput = Cast<URLInputComponent>(PlayerController->InputComponent))
	{
		// 绑定并保存返回的句柄
		FInputBindingHandle Handle = RLInput->BindAction(
			BackAction.LoadSynchronous(),
			ETriggerEvent::Started,
			this,
			&URL_UIManagerSubsystem::HandleBackAction
		);

		InputHandles.Add(PlayerController, Handle);
	}
}

void URL_UIManagerSubsystem::UnbindBackInput(APlayerController* PlayerController)
{
	if (!PlayerController || !InputHandles.Contains(PlayerController)) return;

	if (URLInputComponent* RLInput = Cast<URLInputComponent>(PlayerController->InputComponent))
	{
		// 通过句柄解绑
		RLInput->RemoveBindingByHandle(InputHandles[PlayerController].GetHandle());
		InputHandles.Remove(PlayerController);
	}
}

void URL_UIManagerSubsystem::HandleBackAction()
{
	// 获取当前PlayerController（需根据项目逻辑调整）
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PopWidget(PC);
}

void URL_UIManagerSubsystem::Deinitialize()
{
	//for (auto& Pair : InputHandles)
	//{
	//	if (APlayerController* PC = Pair.Key)
	//	{
	//		UnbindBackInput(PC);
	//	}
	//}
	//InputHandles.Empty();
}

void URL_UIManagerSubsystem::ResetUI()
{
	// 获取 PlayerController（默认只处理本地 0 号）
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	// 移除所有 UI 控件
	for (URL_UserWidget* Widget : WidgetStack)
	{
		if (Widget && Widget->IsInViewport())
		{
			Widget->RemoveFromParent();
		}
	}
	WidgetStack.Empty();

	// 恢复输入模式为游戏模式
	if (PlayerController)
	{
		// 移除 UI 输入映射上下文
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (const UInputMappingContext* LoadedContext = UIContext.LoadSynchronous())
			{
				InputSubsystem->RemoveMappingContext(LoadedContext);
			}
		}

		// 设置为游戏模式
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->SetShowMouseCursor(false);

		// 解绑返回键
		UnbindBackInput(PlayerController);
	}
}

