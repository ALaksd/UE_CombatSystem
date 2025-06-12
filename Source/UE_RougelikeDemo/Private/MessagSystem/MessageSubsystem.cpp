// Fill out your copyright notice in the Description page of Project Settings.


#include "MessagSystem/MessageSubsystem.h"
#include "MessagSystem/Message_Base.h"

void UMessageSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Subscriptions.Empty();
}

void UMessageSubsystem::Deinitialize()
{
	Subscriptions.Empty();
	Super::Deinitialize();
}

void UMessageSubsystem::Subscribe(UObject* Subscriber, TSubclassOf<UMessage_Base> MessageType,
	const FMessageDelegate& Delegate)
{
	if (!Subscriber || !MessageType) return;
	
	FSubscription Subscription(Subscriber,Delegate);
	Subscriptions.Add(MessageType,Subscription);
}

void UMessageSubsystem::Unsubscribe(UObject* Subscriber, TSubclassOf<UMessage_Base> MessageType)
{
	if (!Subscriber || !MessageType) return;

	// 查找消息表中所有MessageType相关的消息
	TArray<FSubscription> FoundSubs;
	Subscriptions.MultiFind(MessageType, FoundSubs);
	// 将与Subscriber匹配的消息注销
	for (int32 i = FoundSubs.Num() - 1; i >= 0; --i)
	{
		if (FoundSubs[i].Subscriber == Subscriber)
		{
			Subscriptions.RemoveSingle(MessageType, FoundSubs[i]);
		}
	}
}

void UMessageSubsystem::Publish(UMessage_Base* Message)
{
	if (!Message) return;

	TSubclassOf<UMessage_Base> SubMessage = Message->GetClass();
	TArray<FSubscription> FoundSubs;
	Subscriptions.MultiFind(SubMessage,FoundSubs);

	for (int32 i = FoundSubs.Num() - 1; i >= 0; --i)
	{
		if (FoundSubs[i].Subscriber.IsValid())
		{
			FoundSubs[i].Delegate.ExecuteIfBound(Message);
		}
		else
		{
			Subscriptions.RemoveSingle(SubMessage, FoundSubs[i]);
		}
	}
}

