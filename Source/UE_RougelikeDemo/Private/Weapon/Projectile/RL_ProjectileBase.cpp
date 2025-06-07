#include "Weapon/Projectile/RL_ProjectileBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/RL_DamageInterface.h"
#include "Components/StaticMeshComponent.h"
#include "GAS/RL_AbilitySystemLibrary.h"

ARL_ProjectileBase::ARL_ProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCom = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereCom);

	USceneComponent* ParentRoot = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("RootComponent")));
	if (ParentRoot)
		ParentRoot->DestroyComponent(); // 销毁父类根组件

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshCom"));
	Mesh->SetupAttachment(SphereCom);
	
	ProjectileCom = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	
}

void ARL_ProjectileBase::FireProjectile()
{
	// 移除附加关系
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	// 以目标actor与箭矢自身连线为速度方向
	if (TargetActor)
		ProjectileCom->Velocity = (TargetActor->GetActorLocation()-GetActorLocation()).GetSafeNormal()*MoveSpeed;
	else
	{
		if (WeaponOwner)
			ProjectileCom->Velocity = WeaponOwner->GetActorForwardVector()*MoveSpeed;
		else
			ProjectileCom->Velocity = GetActorForwardVector()*MoveSpeed;
	}

	this->SetLifeSpan(LifeTime);
}

void ARL_ProjectileBase::InitProjectile(float Damage_T, FGameplayTag DamageTag_T)
{
	this->Damage = Damage_T;
	this->DamageTag = DamageTag_T;
}

void ARL_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SphereCom->OnComponentBeginOverlap.AddDynamic(this,&ARL_ProjectileBase::OnComponentBeginOverlap);

	/// 初始化抛射物的伤害
	// 创建临时GE修改属性
	FGameplayEffectContextHandle Context = WeaponASC->MakeEffectContext();
	Context.AddSourceObject(this);

	UGameplayEffect* GE = NewObject<UGameplayEffect>();
	GE->DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo DamageMod;
	DamageMod.Attribute = UAS_Weapon::GetDamageAttribute();
	DamageMod.ModifierOp = EGameplayModOp::Override;
	DamageMod.ModifierMagnitude = FScalableFloat(Damage);
	GE->Modifiers.Add(DamageMod);

	FGameplayModifierInfo DamageMultiplier;
	DamageMultiplier.Attribute = UAS_Weapon::GetDamageMultiplierAttribute();
	DamageMultiplier.ModifierOp = EGameplayModOp::Override;
	DamageMultiplier.ModifierMagnitude = FScalableFloat(1);
	GE->Modifiers.Add(DamageMultiplier);

	FGameplayModifierInfo SkillAmplification;
	SkillAmplification.Attribute = UAS_Weapon::GetSkillAmplificationAttribute();
	SkillAmplification.ModifierOp = EGameplayModOp::Override;
	SkillAmplification.ModifierMagnitude = FScalableFloat(0);
	GE->Modifiers.Add(SkillAmplification);

	WeaponASC->ApplyGameplayEffectToSelf(GE, 1.0f, Context);
}

void ARL_ProjectileBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->ActorHasTag(AttackActorTag))
		return;

	// if (IRL_DamageInterface* DamageInterface = Cast<IRL_DamageInterface>(OtherActor))
	// {
	// 	DamageSpecHandle = WeaponASC->MakeOutgoingSpec(DamageEffect,WeaponLevel,WeaponASC->MakeEffectContext());
	// 	DamageInterface->TakeDamage(DamageSpecHandle);
	//
	// 	Destroy();
	// }

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	UAbilitySystemComponent* SourceASC = WeaponASC;

	if (TargetASC && SourceASC)
	{
		FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
		TSubclassOf<UGameplayEffect> DamageEffectClass = DamageEffect;
		FGameplayTag DamageTag_T = DamageTag;
		float Damage_T = Damage;
	
		URL_AbilitySystemLibrary::ApplyDamageByMagnitude(SourceASC,TargetASC,Context,DamageEffectClass,DamageTag,Damage);

		Destroy();
	}
}
