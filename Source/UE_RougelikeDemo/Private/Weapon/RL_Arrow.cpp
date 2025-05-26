// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/RL_Arrow.h"
#include "Components/BoxComponent.h"
#include "Components/MeshComponent.h"

// Sets default values
ARL_Arrow::ARL_Arrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;
	ArrowStaitcMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowStaitcMesh"));
	ArrowStaitcMesh->SetupAttachment(RootComponent);

	NiagaraAttachComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NiagaraLoction"));
	NiagaraAttachComponent->SetupAttachment(ArrowStaitcMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bAutoActivate = false;  // 默认情况下不自动激活

	// 设置初始参数（可根据需要调整）
	ProjectileMovement->InitialSpeed = 3000.f;  // 箭的初始速度
	ProjectileMovement->MaxSpeed = 3000.f;      // 箭的最大速度
	ProjectileMovement->bRotationFollowsVelocity = true;  // 使箭的方向随着速度变化
	ProjectileMovement->ProjectileGravityScale = 0.5f;     // 重力缩放，1为正常重力
}

// Called when the game starts or when spawned
void ARL_Arrow::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ARL_Arrow::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ARL_Arrow::OnOverlapEnd);
}

void ARL_Arrow::OnOverlapBegin(UPrimitiveComponent* OverlapedComp, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	if (!OtherActor->ActorHasTag("Player"))
	{
		if (ProjectileMovement)
		{
			ProjectileMovement->DestroyComponent();
		}
		UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin"));
		//记得物体应用重叠事件
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttachToComponent(HitResult.GetComponent(), FAttachmentTransformRules::KeepWorldTransform);
		
		//播放声音
		if (ArrowHitSound && GetWorld())
		{
			UGameplayStatics::PlaySoundAtLocation(this, ArrowHitSound, HitResult.Location);
		}
		//添加特效
		// 检查是否存在ParticleEffect
		if (ParticleEffect)
		{
			// 在Actor的位置生成粒子特效
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect, HitResult.Location);
		}
		//添加冲量
		if (OtherComponent->IsSimulatingPhysics())
		{
			OtherComponent->AddImpulse(ArrowVelocity, NAME_None,true);
			// 销毁箭头
			Destroy();
		}
	}
}

void ARL_Arrow::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ARL_Arrow::Fire(FVector Direction,float Strength)
{
	// 计算发射方向的旋转
	FRotator ArrowRotation = Direction.Rotation();

	// 设置箭的旋转，使其面朝发射方向
	BoxCollision->SetRelativeRotation(ArrowRotation);

	// 激活抛体运动组件
	ProjectileMovement->Activate();

	// 设置发射方向,速度和重力缩放
	ArrowVelocity = Direction * FMath::Lerp(MinSpeed, MaxSpeed, Strength);
	ProjectileMovement->Velocity = ArrowVelocity;
	ProjectileMovement->ProjectileGravityScale = FMath::Lerp(MaxGravity, MinGravity, Strength);

	//设置碰撞
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//播放声音
	if (ArrowShootSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ArrowShootSound, GetActorLocation());
	}

	//生成特效
	if (NiagaraEffect && NiagaraAttachComponent) // 确保 NiagaraEffect 和 NiagaraAttachComponent 有效
	{
		ENCPoolMethod PoolingMethod = ENCPoolMethod::None; // 初始化 ENCPoolMethod

		UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraEffect,                       // 引用的 Niagara 特效
			NiagaraAttachComponent,              // 要附加的组件
			NAME_None,                           // 没有特定插槽则使用 NAME_None
			FVector::ZeroVector,                 // 位置偏移
			FRotator::ZeroRotator,               // 旋转
			FVector(1.0f),                       // 缩放
			EAttachLocation::KeepRelativeOffset, // 附加规则 (正确的位置)
			false,                               // 是否自动销毁
			PoolingMethod,                       // 池化方法
			true                                 // 手动管理生命周期
		);
	}
	//设置生命周期
	SetLifeSpan(10.0f);
}

//// Called every frame
//void ARL_Arrow::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

