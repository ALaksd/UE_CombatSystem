// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MessageSubsystem.generated.h"

/**
 * 消息系统
 *
 * 通过Message查找出相应的FSubscription得到delegate
 */

DECLARE_DYNAMIC_DELEGATE_OneParam(FMessageDelegate, UMessage_Base*, Message);

struct FSubscription
{
	TWeakObjectPtr<UObject> Subscriber;
	FMessageDelegate Delegate;

	FSubscription()
		: Subscriber(nullptr) , Delegate(){}
	FSubscription(TWeakObjectPtr<UObject> Subscriber ,FMessageDelegate Delegate)
		: Subscriber(Subscriber), Delegate(Delegate){}

	// 运算符重载
	bool operator==(const FSubscription& Other) const
	{
		return Subscriber == Other.Subscriber && Delegate == Other.Delegate;
	}
};

class UMessage_Base;


UCLASS()
class UE_ROUGELIKEDEMO_API UMessageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// 订阅消息
	UFUNCTION(BlueprintCallable, Category = "Message System")
	void Subscribe(UObject* Subscriber, TSubclassOf<UMessage_Base> MessageType, const FMessageDelegate& Delegate);

	// 取消订阅
	UFUNCTION(BlueprintCallable, Category = "Message System")
	void Unsubscribe(UObject* Subscriber, TSubclassOf<UMessage_Base> MessageType);

	// 发布消息
	UFUNCTION(BlueprintCallable, Category = "Message System")
	void Publish(UMessage_Base* Message);

protected:
	//初始化子系统
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	//消息表
	TMultiMap<TSubclassOf<UMessage_Base>, FSubscription> Subscriptions;

};
