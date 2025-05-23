// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Interface/RL_CombatInterface.h"
#include "Interface/RL_EnemyInterface.h"
#include "UI/WidgetController/RL_OverlayWidgetController.h"
#include "GameFramework/Character.h"
#include "Interface/RL_DamageInterface.h"
#include "Enemy_Base.generated.h"


class UBehaviorTree;
class ARL_AIController;
class UWidgetComponent;
class URL_EnemyMovementComponent;
class USplineComponent;
class UNiagaraComponent;
class UAS_Enemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLock, bool, bLock);

UCLASS()
class UE_ROUGELIKEDEMO_API AEnemy_Base : public ACharacter, public IAbilitySystemInterface,
	public IRL_CombatInterface,public IRL_EnemyInterface,public IRL_DamageInterface
{
	GENERATED_BODY()

public:
	AEnemy_Base();

	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Components")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
private:
	
	//属性
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeSet> AttributeSet;

	// 存储状态相关的标签(暂时只放破防与蹒跚状态)
	FGameplayTagContainer StateTags;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ChangeLockPointColor(bool bIsRed);
	// 正面处决动画
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Animation | State")
	TObjectPtr<UAnimMontage> Aim_Execute_F;
	// 背面处决动画
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Animation | State")
	TObjectPtr<UAnimMontage> Aim_Execute_B;
	// 敌人是否发现玩家的标识
	bool bIsFindPlayer;
	// 处理处决
	void Execute(bool bIsForward);

	//骨骼抖动,因为涉及到时间轴，所以在蓝图实现
	UFUNCTION(BlueprintImplementableEvent)
	void PlayBoneShake(FName BoneName, FVector ShakeDirection, float Magnitude);
	
	// 接收伤害
	virtual void TakeDamage(const FGameplayEffectSpecHandle& DamageHandle) const override;
	
	inline  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return AbilitySystemComponent;}

	/** CombatInterface */
	virtual UAnimMontage* GetHitReactMotange_Implementation() override;
	virtual  UAnimMontage* GetLightHitReactFrontMontage_Implementation() const override;
	virtual  UAnimMontage* GetLightHitReactBackMontage_Implementation() const override;
	virtual  UAnimMontage* GetLightHitReactLeftMontage_Implementation() const override;
	virtual  UAnimMontage* GetLightHitReactRightMontage_Implementation() const override;
	virtual  UAnimMontage* GetHeavyHitReactMontage_Implementation() const override;

	virtual void Die_Implementation() override;
	FORCEINLINE virtual bool isDead_Implementation() const override {return bDead;}
	virtual void KnockBack_Implementation(const FVector& KonckBackImpulse) override;
	/** End ComvatInterface */

	/** EnemyInterface */
	virtual AActor* GetCombatTarget_Implementation()const override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual UNiagaraComponent* GetRedAttackNiagaraComponent_Implementation() const override;
	virtual void SetHealthBarVisible_Implementation(bool bVisible) const override;
	virtual void SetLockTarget_Implementation(bool bInLock) override;
	virtual void SetLockUIRed_Implementation(bool bInRedLock) override;
	virtual void SetHitShake_Implementation(FName BoneName, FVector ShakeDirection, float Magnitude);
	virtual UAS_Enemy* GetEnemyAttributeSet_Implementation() const;
	/** End EnemyInterface */

	/*-------------------------破防状态相关-------------------------*/
	
	
	// 处理破防相关
	void GuardBroken();
	// 处理蹒跚相关
	void Staggered();

	// 破防状态
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AnimationUpdate | State")
	bool bIsGuardBroken = false;
	// 蹒跚状态
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AnimationUpdate | State")
	bool bIsStaggered = false;


	/*-------------------------破防状态相关-------------------------*/
	
	FORCEINLINE UStaticMeshComponent* GetWeaponStaticComponnent() { return WeaponStaticMeshComponent; }
	FORCEINLINE UNiagaraComponent* GetNiagaraComponent() { return RedAttackNiagaraComponent; }

	/** Spawner */

	//设置巡逻点
	void InitializePatrol(USplineComponent* NewPatrolSpline);

	/** 锁定 */
	UPROPERTY(BlueprintAssignable)
	FOnLock OnLock;
	
protected:
	// 处决用
	bool bIsExecuting = false;

	/*-------------------------破防状态相关-------------------------*/
	
	// 蹒跚时间
	UPROPERTY(EditDefaultsOnly,Category="Attribute | State")
	float StaggeredTime;
	// 破防时间
	UPROPERTY(EditDefaultsOnly,Category="Attribute | State")
	float GuardBrokenTime;

	// 体力减少回复时间
	UPROPERTY(EditDefaultsOnly,Category="Attribute | State")
	float StaminaReduceTime;
	// 韧性减少回复时间
	UPROPERTY(EditDefaultsOnly,Category="Attribute | State")
	float ResilienceReduceTime;
	
	FTimerHandle StaminaReduceTimer;
	FTimerHandle StaggeredTimer;
	FTimerHandle GuardBrokenTimer;
	FTimerHandle ResilienceReduceTimer;
	// 体力减少回调
	void StaminaReduceCallBack();
	// 韧性减少回调
	void ResilienceReduceCallBack();

	/*-------------------------破防状态相关-------------------------*/
	
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;

	// 回复体力,韧性
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="GE_Restore")
	TSubclassOf<UGameplayEffect> GE_RestoreStamina;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="GE_Restore")
	TSubclassOf<UGameplayEffect> GE_RestoreResilience;
	
	/** AI*/

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BeahabviorTree;

	UPROPERTY()
	TObjectPtr<ARL_AIController> RLAIController;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> WeaponStaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UNiagaraComponent> RedAttackNiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<URL_EnemyMovementComponent> EnemyMovementComponent;
	
	/** UI */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxStaminaChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnStaminaChanged;

	/** Hit */

	//HitReact标签变化回调
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	bool bHitReacting;

	float BaseWalkSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "RLCharacter|AnimMontage")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(BlueprintReadWrite)
	bool bDead;

	/** Ability */
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> Abilites;

	void AddCharacterAbilities();

private:
	void InitAbilityActorInfo();

	void InitializeAttribute();

	// 添加标签
	void AddTag(FName Tag);
	// 移除标签
	void RemoveTag(FName Tag);

	// 体力变化回调
	void StaminaAttributeChangeCallback(const FOnAttributeChangeData& Data);
	// 韧性变化回调
	void ResilienceAttributeChangeCallback(const FOnAttributeChangeData& Data);

	bool bLock;
	bool bRedLock;
};


