
#pragma once

#include "CoreMinimal.h"
#include "InteractableInterface.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

class UInteractionDataAsset;
class UBoxComponent;

UCLASS()
class UE_ROUGELIKEDEMO_API AInteractableActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();

	// 碰撞区域，用于触发交互检测
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact")
	UBoxComponent* InteractCollision;

	// 交互行为数据
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interact")
	UInteractionDataAsset* InteractionData;

	// Overlap 回调
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
					  AActor* OtherActor,
					  UPrimitiveComponent* OtherComp,
					  int32 OtherBodyIndex);
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 实现交互接口
	virtual void Interact_Implementation(AActor* Interactor) override;
};
