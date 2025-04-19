// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/RL_AttributeWidgetController.h"
#include "GAS/AS/AS_Player.h"
#include "RL_GameplayTags.h"

void URL_AttributeWidgetController::BroadcastInitialValue()
{
	UAS_Player* AS = CastChecked<UAS_Player>(AttributeSet);

	check(AttributeInfo);

	//ѭ���������еı�ǩ�Ͷ�Ӧ���ԣ��������Բ���ÿ�����Զ��㲥һ��
	for (auto& Pair : AS->TagsToAttribute)
	{
		FGameplayAttribute Attr = Pair.Value.Execute();
		float Value = AbilitySystemComponent->GetNumericAttribute(Attr);

		FRLAttributeInfo Info = AttributeInfo->FindAttributeInfoFromTag(Pair.Key);
		Info.AttributeValue = Value;
		AttributeInfoDelegate.Broadcast(Info);

	}
}

void URL_AttributeWidgetController::BindCallbacksToDependencies()
{
	UAS_Player* AS = CastChecked<UAS_Player>(AttributeSet);
	for (auto& Pair : AS->TagsToAttribute)
	{
		FGameplayAttribute Attribute = Pair.Value.Execute();
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
			[this, Tag = Pair.Key](const FOnAttributeChangeData& Data)
			{
				FRLAttributeInfo Info = AttributeInfo->FindAttributeInfoFromTag(Tag);
				Info.AttributeValue = Data.NewValue;
				AttributeInfoDelegate.Broadcast(Info);
			}
		);
	}
}
