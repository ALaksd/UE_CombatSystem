// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "RL_UIManagerSubsystem.generated.h"

/**
 * 用于管理UI的创建和销毁
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_UIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// 初始化子系统,初始化时间与GameInstance相同
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//创建新的UI
	UFUNCTION(BlueprintCallable)
	UUserWidget* AddNewWidget(TSubclassOf<UUserWidget> WidgetClass, APlayerController* PlayerController);

	//创建支持开关的UI
	UFUNCTION(BlueprintCallable)
	void ToggleWidget(TSubclassOf<UUserWidget> WidgetClass, APlayerController* PlayerController);

protected:
	// 配置需要使用的Input资源（在编辑器设置）
	UPROPERTY(BlueprintReadWrite, Category = "Input")
	TSoftObjectPtr<UInputAction> BackAction;

	UPROPERTY(BlueprintReadWrite, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> UIContext;

private:
	UPROPERTY()
	TArray<UUserWidget*> WidgetStack; // UI栈

	// 压入新UI到栈顶
	UUserWidget* PushWidget(TSubclassOf<UUserWidget> WidgetClass, APlayerController* PlayerController);
	// 弹出栈顶UI
	void PopWidget(APlayerController* PlayerController);

	// 绑定返回键输入
	void BindBackInput(APlayerController* PlayerController);
	void UnbindBackInput(APlayerController* PlayerController);

	// 处理返回键逻辑
	void HandleBackAction();

	TMap<APlayerController*, FInputBindingHandle> InputHandles;
};
