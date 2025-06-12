// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_ROUGELIKEDEMO_API UInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInteractComponent();

	// 玩家尝试交互（在输入中调用此函数）
	UFUNCTION(BlueprintCallable, Category = "Interact")
	void TryInteract();

	// 注册/注销可交互对象（由交互物体通知调用）
	void RegisterInteractable(AActor* InteractableActor);
	void UnregisterInteractable(AActor* InteractableActor);

protected:
	virtual void BeginPlay() override;

	// 当前重叠的可交互对象列表
	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> OverlappingInteractables;

	// 获取当前优先交互对象（可根据距离、优先级等拓展）
	AActor* GetBestInteractable() const;

	// 显示/隐藏交互提示（可蓝图扩展）
	UFUNCTION(BlueprintImplementableEvent)
	void OnInteractTargetChanged(AActor* NewTarget);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
