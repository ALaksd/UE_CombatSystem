// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RL_BaseWeapon.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Interface/RL_DamageInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UE_RougelikeDemo/InventorySystem/RLItemFragment_EquipDynamicData.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_WeaponLevelData.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemInstance.h"
#include "UE_RougelikeDemo/UE_RougelikeDemo.h"
#include <GAS/RL_AbilitySystemLibrary.h>
#include <Interface/RL_EnemyInterface.h>
#include <Interface/RL_PlayerInterface.h>
#include "GAS/AS/AS_Player.h"
#include <System/RL_SanitySubsystem.h>

// Sets default values
ARL_BaseWeapon::ARL_BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	
	WeaponASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	WeaponAttribute = CreateDefaultSubobject<UAS_Weapon>(TEXT("AttributeSet"));
}


void ARL_BaseWeapon::SetWeaponLevel(int32 NewLevel)
{
	//获取武器等级数据的Fragment
	if (!ItemInstance) return;

	const URLItemFragment_WeaponLevelData* WeaponLevelDataFrag = ItemInstance->FindFragmentByClass<URLItemFragment_WeaponLevelData>();
	if (!WeaponLevelDataFrag) return;

	if (NewLevel > WeaponLevelDataFrag->MaxLevel)
	{
		return;
	}
	WeaponLevel = NewLevel;


	if (WeaponAttribute && WeaponASC)
	{
		// 创建临时GE修改属性
		FGameplayEffectContextHandle Context = WeaponASC->MakeEffectContext();
		Context.AddSourceObject(this);

		UGameplayEffect* GE = NewObject<UGameplayEffect>();
		GE->DurationPolicy = EGameplayEffectDurationType::Instant;

		//仅设置Damage
		FGameplayModifierInfo DamageMod;
		DamageMod.Attribute = UAS_Weapon::GetDamageAttribute();
		DamageMod.ModifierOp = EGameplayModOp::Override;
		DamageMod.ModifierMagnitude = FScalableFloat(WeaponLevelDataFrag->GetWeaponLevelData(WeaponLevel).BaseDamage);
		GE->Modifiers.Add(DamageMod);

		WeaponASC->ApplyGameplayEffectToSelf(GE, 1.0f, Context);
	}
}

void ARL_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ARL_BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bCombat)
	{
		GetCurrentPointsLocation();
		 
		//碰撞检测参数
		EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::ForDuration;
		FLinearColor TraceColor = FLinearColor::Red;
		FLinearColor TraceHitColor = FLinearColor::Green;
		float DrawTime = 0.5;
		
		for (int i=0;i<LastPoints.Num();i++)
		{
			//多重射线检测
			FVector Start = LastPoints[i];
			FVector End =  CurrentPoints[i];
			TArray<FHitResult> OutHits;
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(WeaponOwner);
			bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),Start,End,15.f,ObjectTypes,false,ActorsToIgnore,DrawDebugType,OutHits,true,TraceColor,TraceHitColor,DrawTime);
			//bool bHit = GetWorld()->LineTraceMultiByChannel(OutHits, Start, End, ECC_Enemy);

			if (bHit)
			{
				for (int j = 0; j < OutHits.Num(); j++)
				{
					AActor* HitActor = OutHits[j].GetActor();

					if (!HitActors.Contains(HitActor))//此Actor没被检测过
					{
						HitActors.Add(HitActor);
						//执行伤害逻辑
						if (IRL_DamageInterface* DamageInterface = Cast<IRL_DamageInterface>(HitActor))
						{
							//传入自定义的参数
							FGameplayEffectContextHandle Context = WeaponASC->MakeEffectContext();
							//FVector KonckBackVector = (WeaponOwner->GetActorLocation() - HitActor->GetActorLocation()).GetSafeNormal();
							FVector KonckBackVector = OutHits[j].ImpactNormal;
							float DamageMultiplier = WeaponAttribute->GetDamageMultiplier();
							FVector KonckImpulse = KonckBackVector * DamageMultiplier * KnockDistance;

							//传入击退参数
							URL_AbilitySystemLibrary::SetKonckBackImpulse(Context, KonckImpulse);
							//传入击中骨骼名字参数
							URL_AbilitySystemLibrary::SetHitBoneName(Context, OutHits[j].BoneName);


							//执行GameplayCue
							FGameplayCueParameters CueParams;
							CueParams.Instigator = WeaponOwner; //造成伤害者
							CueParams.Location = OutHits[j].ImpactPoint; //击中位置
							CueParams.Normal = OutHits[j].ImpactNormal;  //击中法向
							CueParams.PhysicalMaterial = OutHits[j].PhysMaterial;  //击中物理材质
							CueParams.NormalizedMagnitude = DamageMultiplier;  //击中强度,根据武器的倍率来计算

							IAbilitySystemInterface* TargetAbilityStystemInterface = Cast<IAbilitySystemInterface>(HitActor);
							if (TargetAbilityStystemInterface)
							{
								UAbilitySystemComponent* TargetASC = TargetAbilityStystemInterface->GetAbilitySystemComponent();
								TargetASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.MeeleHit"), CueParams);
							}
						
							RestoreAttachResourceAndSanity(DamageMultiplier);
					


							DamageSpecHandle = WeaponASC->MakeOutgoingSpec(DamageEffet, WeaponLevel, Context);

							DamageInterface->TakeDamage(DamageSpecHandle);

							IRL_EnemyInterface::Execute_SetHitShake(HitActor, OutHits[j].BoneName, KonckBackVector, DamageMultiplier * KnockDistance);
						}
					}
				}
			}
		}
		SetLastPointsLocation();
		
	}
}

void ARL_BaseWeapon::RestoreAttachResourceAndSanity(float DamageMultiplier)
{
	// 创建动态GE增加玩家的气势值
	IAbilitySystemInterface* SourceAbilityStystemInterface = Cast<IAbilitySystemInterface>(WeaponOwner);
	if (SourceAbilityStystemInterface)
	{
		UAbilitySystemComponent* SourceASC = SourceAbilityStystemInterface->GetAbilitySystemComponent();
		UGameplayEffect* DynamicParryGE = NewObject<UGameplayEffect>(SourceASC, FName(TEXT("DynamicParryGE")));
		DynamicParryGE->DurationPolicy = EGameplayEffectDurationType::Instant; // 即时生效

		FGameplayModifierInfo& Modifier = DynamicParryGE->Modifiers.AddDefaulted_GetRef();
		if (WeaponOwner->Implements<URL_PlayerInterface>())
		{
			UAS_Player* AS = IRL_PlayerInterface::Execute_GetPlayerAS(WeaponOwner);
			if (AS)
			{
				Modifier.Attribute = FGameplayAttribute(AS->GetAttachResourceAttribute());
				Modifier.ModifierOp = EGameplayModOp::Additive; // 修改类型：叠加
				FScalableFloat Magnitude;
				Magnitude.Value = 1.f * DamageMultiplier; // 暂时10 * 武器倍率
				Modifier.ModifierMagnitude = Magnitude;

				// 创建效果规格并应用
				FGameplayEffectContextHandle Context = SourceASC->MakeEffectContext();
				SourceASC->ApplyGameplayEffectToSelf(DynamicParryGE, 1.f, Context);
			}
		}
	}

	//恢复理智
	URL_SanitySubsystem* SanitySubsystem = GetGameInstance()->GetSubsystem<URL_SanitySubsystem>();
	if (SanitySubsystem)
	{
		SanitySubsystem->RestoreSanity(5.f * DamageMultiplier);
	}

}

void ARL_BaseWeapon::StartCombat()
{
	bCombat = true;
	
	//创建GameplayEffect
	DamageSpecHandle = WeaponASC->MakeOutgoingSpec(DamageEffet,WeaponLevel,WeaponASC->MakeEffectContext());

}

void ARL_BaseWeapon::EndCombat()
{
	bCombat = false;
	LastPoints.Empty();
	HitActors.Empty();
}

void ARL_BaseWeapon::GetCurrentPointsLocation()
{
	CurrentPoints.Empty();

	FName SocketName0 = FName("Socket_Attack_0");
	FName SocketName1 = FName("Socket_Attack_1");
	FName SocketName2 = FName("Socket_Attack_2");
	FName SocketName3 = FName("Socket_Attack_3");
	FName SocketName4 = FName("Socket_Attack_4");

	CurrentPoints.Add(Mesh->GetSocketLocation(SocketName0));
	CurrentPoints.Add(Mesh->GetSocketLocation(SocketName1));
	CurrentPoints.Add(Mesh->GetSocketLocation(SocketName2));
	CurrentPoints.Add(Mesh->GetSocketLocation(SocketName3));
	CurrentPoints.Add(Mesh->GetSocketLocation(SocketName4));
}

void ARL_BaseWeapon::SetLastPointsLocation()
{
	LastPoints.Empty();

	for (int i=0;i<CurrentPoints.Num();i++)
	{
		LastPoints.Add(CurrentPoints[i]);
	}
	
}

