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


	
	// 接收伤害
	virtual void TakeDamage(const FGameplayEffectSpecHandle& DamageHandle) const override;
	
	inline  virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return AbilitySystemComponent;}

	/** CombatInterface */
	virtual UAnimMontage* GetHitReactMotange_Implementation() override;
	virtual void Die_Implementation() override;

	/** End ComvatInterface */

	/** EnemyInterface */
	virtual AActor* GetCombatTarget_Implementation()const override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual UNiagaraComponent* GetRedAttackNiagaraComponent_Implementation() const override;

	/** End EnemyInterface */

	// 体力减少回调
	UFUNCTION(BlueprintImplementableEvent)
	void StaminaReduceCallBack();
	// 韧性减少回调
	UFUNCTION(BlueprintImplementableEvent)
	void ResilienceReduceCallBack();

	// 破防动画
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Animation | State")
	TObjectPtr<UAnimMontage> Aim_GuardBroken;

	// 蹒跚动画
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Animation | State")
	TObjectPtr<UAnimMontage> Aim_Staggered;

	
	
	// 处理破防相关
	void GuardBroken();
	// 处理蹒跚相关
	void Staggered();

UFUNCTION(BlueprintCallable)
	void GuardBrokenCallBack();
	UFUNCTION(BlueprintCallable)
	void StaggeredCallBack();

	// 破防状态
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AnimationUpdate | State")
	bool bIsGuardBroken = false;
	// 蹒跚状态
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="AnimationUpdate | State")
	bool bIsStaggered = false;

	FORCEINLINE UStaticMeshComponent* GetWeaponStaticComponnent() { return WeaponStaticMeshComponent; }
	FORCEINLINE UNiagaraComponent* GetNiagaraComponent() { return RedAttackNiagaraComponent; }
	
protected:
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

	//样条线组件，用于巡逻
	UPROPERTY(VisibleAnywhere, Category = "AI|Patrol")
	TObjectPtr<USplineComponent> PatrolSpline;

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
	FOnAttributeChangedSignature OnStaminaChanged;

	/** Hit */

	//HitReact标签变化回调
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	bool bHitReacting;

	float BaseWalkSpeed = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "RLCharacter|AnimMontage")
	TObjectPtr<UAnimMontage> HitReactMontage;

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
};


