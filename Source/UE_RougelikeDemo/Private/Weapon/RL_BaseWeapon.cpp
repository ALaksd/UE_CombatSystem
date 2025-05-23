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


							////执行GameplayCue
							//FGameplayCueParameters CueParams;
							//CueParams.EffectContext = Context;
							//CueParams.RawMagnitude = DamageMultiplier * KnockDistance; // 力量
							//IAbilitySystemInterface* AbilityStystemInterface = Cast<IAbilitySystemInterface>(HitActor);
							//if (AbilityStystemInterface)
							//{
							//	UAbilitySystemComponent* TargetASC = AbilityStystemInterface->GetAbilitySystemComponent();
							//	TargetASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag("GameplayCue.HitReact"), CueParams);
							//}
						
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

