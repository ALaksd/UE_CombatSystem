// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CloseCombatComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"


UCloseCombatComponent::UCloseCombatComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	//创建组件
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	//SkeletalMesh->SetupAttachment(this);

}


void UCloseCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	User = Cast<ACharacter>(GetOwner());

	if (User && SkeletalMesh)
	{
		//将武器绑到使用者手上
		FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepRelative,true);
		SkeletalMesh->AttachToComponent(User->GetMesh(),AttachmentTransformRules,FName("Socket_Weapon_Sword"));
		
		// TArray<FName> SoketeNames = SkeletalMesh->GetAllSocketNames();
		// for (int i =0; i < SoketeNames.Num(); i++)
		// {
		// 	Points.Add(SkeletalMesh->GetSocketByName(SoketeNames[i])->GetSocketLocation(SkeletalMesh));
		// }
	}
	
}

void UCloseCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCombat)
	{
		for (int i = 0; i < Points.Num() - 1; i++)
		{
			FVector Start = Points[i];
			FVector End =  Points[i + 1];
			//可见通道
			ETraceTypeQuery TraceChannel = TraceTypeQuery1;
			TArray<AActor*> ActorsToIgnore;
			EDrawDebugTrace::Type DrawDebugType = EDrawDebugTrace::None;
			FHitResult OutHit;
			FLinearColor TraceColor = FLinearColor::Red;
			FLinearColor TraceHitColor = FLinearColor::Green;
			float DrawTime = 0;

			//忽略攻击者
			ActorsToIgnore.Add(User);
			UKismetSystemLibrary::LineTraceSingle(GetWorld(),Start,End,TraceChannel,false,ActorsToIgnore,DrawDebugType,OutHit,true,TraceColor,TraceHitColor,DrawTime);

			//射线检测判定成功
			if (OutHit.bBlockingHit)
			{
				//TODO:执行伤害逻辑
			}
		}
		
	}


	
}

void UCloseCombatComponent::StartCombat()
{
	bCombat=true;
}

void UCloseCombatComponent::EndCombat()
{
	bCombat=false;
}

