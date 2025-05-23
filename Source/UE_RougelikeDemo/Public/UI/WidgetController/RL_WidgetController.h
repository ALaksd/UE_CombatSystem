// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RL_WidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UAS_Player;
class ARL_PlayerState;
class UASC_Base;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		:PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * 
 */
UCLASS()
class UE_ROUGELIKEDEMO_API URL_WidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(FWidgetControllerParams& WCParams);

	//广播属性变化
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValue();

	//GAS回调函数(用于GAS通知属的变化委托的回调函数)
	virtual void BindCallbacksToDependencies();

	//在空间销毁时调用
	UFUNCTION(BlueprintCallable)
	virtual void UnbindAllDelegate();

	UFUNCTION(BlueprintCallable)
	inline UASC_Base* GetPlayerASC() const;
protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UFUNCTION(BlueprintCallable)
	inline UAS_Player* GetPlayerAS() const;

	UFUNCTION(BlueprintCallable)
	inline ARL_PlayerState* GetPlayerState() const;
};
