#pragma once

#include "CoreMinimal.h"
#include "Weapon/RL_BaseWeapon.h"
#include "RL_ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class UE_ROUGELIKEDEMO_API ARL_ProjectileBase : public ARL_BaseWeapon
{
	GENERATED_BODY()
	
public:	
	ARL_ProjectileBase();

	// 发射
	void FireProjectile();

	/// 
	/// @param Damage_T 伤害数值
	/// @param DamageTag_T 是否红光攻击
	void InitProjectile(float Damage_T,FGameplayTag DamageTag_T);

	UPROPERTY()
	AActor* TargetActor;
	
protected:
	/*---------------------组件---------------------*/

	// 球形碰撞组件
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	TObjectPtr<USphereComponent> SphereCom;

	// 网格体组件
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	TObjectPtr<UStaticMeshComponent> Mesh;

	// 抛物体组件
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	TObjectPtr<UProjectileMovementComponent> ProjectileCom;
	
	/*---------------------组件---------------------*/

	/*---------------------属性---------------------*/

	UPROPERTY(EditDefaultsOnly,Category="Attribute | Base")
	float LifeTime;

	// 发射物移动速度
	UPROPERTY(EditDefaultsOnly,Category="Attribute | Base")
	float MoveSpeed;
		
	// 发射物初始移动速度
	UPROPERTY(EditDefaultsOnly,Category="Attribute | Base")
	float InitSpeed;
	
	// 发射物最大移动速度
	UPROPERTY(EditDefaultsOnly,Category="Attribute | Base")
	float MaxSpeed;

	// 发射物伤害
	UPROPERTY(EditDefaultsOnly,Category="Attribute | Damage")
	float Damage;

	UPROPERTY(EditDefaultsOnly,Category="Attribute | Damage")
	FGameplayTag DamageTag;

	// 要攻击的character的Tag
	UPROPERTY(EditDefaultsOnly,Category="Attribute | Damage")
	FName AttackActorTag;
	/*---------------------属性---------------------*/

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//抛射物击中后产生的物理效果
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, const FHitResult& HitResult);
	
private:
	virtual void BeginPlay() override;
	
};
