
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable_Base.generated.h"

/*
 * 可交互物品基类
 */

UCLASS()
class UE_ROUGELIKEDEMO_API AInteractable_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	virtual void TryInteract();

};
