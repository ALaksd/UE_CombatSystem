#include "Weapon/RL_ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interface/RL_DamageInterface.h"

ARL_ProjectileBase::ARL_ProjectileBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCom = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereCom);

	USceneComponent* ParentRoot = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("RootComponent")));
	if (ParentRoot)
		ParentRoot->DestroyComponent(); // 销毁父类根组件
	
	Mesh->SetupAttachment(SphereCom);
	ProjectileCom = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	
}

void ARL_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SphereCom->OnComponentBeginOverlap.AddDynamic(this,&ARL_ProjectileBase::OnComponentBeginOverlap);
	ProjectileCom->InitialSpeed = InitSpeed;
	ProjectileCom->MaxSpeed=MaxSpeed;


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

	if (IRL_DamageInterface* DamageInterface = Cast<IRL_DamageInterface>(OtherActor))
	{
		DamageSpecHandle = WeaponASC->MakeOutgoingSpec(DamageEffet,WeaponLevel,WeaponASC->MakeEffectContext());
		DamageInterface->TakeDamage(DamageSpecHandle);
	}
	
}
