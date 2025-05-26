#pragma once

#include "CoreMinimal.h"
#include "RL_BaseWeapon.h"
#include "RL_ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class UE_ROUGELIKEDEMO_API ARL_ProjectileBase : public ARL_BaseWeapon
{
	GENERATED_BODY()
	
public:	
	ARL_ProjectileBase();

	
protected:
	/*---------------------组件---------------------*/

	// 球形碰撞组件
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	TObjectPtr<USphereComponent> SphereCom;

	// 抛物体组件
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileCom;
	
	/*---------------------组件---------------------*/

	/*---------------------属性---------------------*/

	// 发射物初始移动速度
	UPROPERTY(EditDefaultsOnly,Category="Attribute | Base")
	float InitSpeed;

	// 发射物最大移动速度
	UPROPERTY(EditDefaultsOnly,Category="Attribute | Base")
	float MaxSpeed;

	// 发射物伤害
	UPROPERTY(EditDefaultsOnly,Category="Attribute | Damage")
	float Damage;

	// 要攻击的chara的Tag
	UPROPERTY(EditDefaultsOnly,Category="Attribute | Damage")
	FName AttackActorTag;
	/*---------------------属性---------------------*/

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
private:
	virtual void BeginPlay() override;
	
};
