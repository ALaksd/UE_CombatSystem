// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RL_Sword.h"

#include "AbilitySystemInterface.h"
#include "GAS/RL_AbilitySystemLibrary.h"
#include "GAS/AS/AS_Player.h"
#include "Interface/RL_DamageInterface.h"
#include "Interface/RL_EnemyInterface.h"
#include "Interface/RL_PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "System/RL_SanitySubsystem.h"
#include <NiagaraFunctionLibrary.h>
#include "NiagaraComponent.h"
#include <RL_CharacterSelectionWidget.cpp>



ARL_Sword::ARL_Sword()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("SwordMesh");
	Mesh->SetupAttachment(GetRootComponent());

	TrailComponent = CreateDefaultSubobject<UNiagaraComponent>("TrailComponent");
	TrailComponent->SetupAttachment(Mesh);
}

void ARL_Sword::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bCombat)
	{
		GetCurrentPointsLocation();
		 
		//碰撞检测参数
		EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
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
							if (WeaponAttribute)
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


								DamageSpecHandle = WeaponASC->MakeOutgoingSpec(DamageEffect, WeaponLevel, Context);
								IRL_EnemyInterface::Execute_SetHitShake(HitActor, OutHits[j].BoneName, KonckBackVector, DamageMultiplier * KnockDistance);

							}

							DamageInterface->TakeDamage(DamageSpecHandle);
						}
					}
				}
			}
		}
		SetLastPointsLocation();
		
	}
}


void ARL_Sword::RestoreAttachResourceAndSanity(float DamageMultiplier)
{
	//恢复理智
	URL_SanitySubsystem* SanitySubsystem = GetGameInstance()->GetSubsystem<URL_SanitySubsystem>();
	if (SanitySubsystem)
	{
		SanitySubsystem->RestoreSanity(5.f * DamageMultiplier);
	}

}


void ARL_Sword::StartCombat()
{
	bCombat = true;
	
	//创建GameplayEffect
	DamageSpecHandle = WeaponASC->MakeOutgoingSpec(DamageEffect,WeaponLevel,WeaponASC->MakeEffectContext());

}

void ARL_Sword::EndCombat()
{
	bCombat = false;
	LastPoints.Empty();
	HitActors.Empty();
}

void ARL_Sword::StartTrailEffect()
{
	if (TrailComponent)
	{
		TrailComponent->Activate(true);
	}
	if (AttackSound)
	{
		UGameplayStatics::PlaySoundAtLocation(WeaponOwner, AttackSound, WeaponOwner->GetActorLocation());
	}
}

void ARL_Sword::StopTrailEffect()
{
	if (TrailComponent)
	{
		TrailComponent->Deactivate();
	}
}

void ARL_Sword::GetCurrentPointsLocation()
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

void ARL_Sword::SetLastPointsLocation()
{
	LastPoints.Empty();

	for (int i=0;i<CurrentPoints.Num();i++)
	{
		LastPoints.Add(CurrentPoints[i]);
	}
	
}
