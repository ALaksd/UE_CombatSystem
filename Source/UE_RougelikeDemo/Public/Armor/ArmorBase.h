// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/RL_ItemInstanceHolder.h"
#include "ArmorBase.generated.h"

UENUM(BlueprintType)
enum class ArmorType : uint8
{
	Helmet,          // 头盔
	Armor,     // 身体甲
};

UCLASS()
class UE_ROUGELIKEDEMO_API AArmorBase : public AActor, public IRL_ItemInstanceHolder
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmorBase();

	//防具种类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ArmorType Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Bone;

	virtual URLInventoryItemInstance* GetItemInstance_Implementation() const override { return ItemInstance; }
	virtual void SetItemInstance_Implementation(URLInventoryItemInstance* NewInstance) override { ItemInstance = NewInstance; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	/** 物品实例 */
	UPROPERTY()
	TObjectPtr<URLInventoryItemInstance> ItemInstance = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
