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
	ProjectileMovement->bAutoActivate = false;  // Ĭ������²��Զ�����

	// ���ó�ʼ�������ɸ�����Ҫ������
	ProjectileMovement->InitialSpeed = 3000.f;  // ���ĳ�ʼ�ٶ�
	ProjectileMovement->MaxSpeed = 3000.f;      // ��������ٶ�
	ProjectileMovement->bRotationFollowsVelocity = true;  // ʹ���ķ��������ٶȱ仯
	ProjectileMovement->ProjectileGravityScale = 0.5f;     // �������ţ�1Ϊ��������
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
		//�ǵ�����Ӧ���ص��¼�
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttachToComponent(HitResult.GetComponent(), FAttachmentTransformRules::KeepWorldTransform);
		
		//��������
		if (ArrowHitSound && GetWorld())
		{
			UGameplayStatics::PlaySoundAtLocation(this, ArrowHitSound, HitResult.Location);
		}
		//�����Ч
		// ����Ƿ����ParticleEffect
		if (ParticleEffect)
		{
			// ��Actor��λ������������Ч
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleEffect, HitResult.Location);
		}
		//��ӳ���
		if (OtherComponent->IsSimulatingPhysics())
		{
			OtherComponent->AddImpulse(ArrowVelocity, NAME_None,true);
			// ���ټ�ͷ
			Destroy();
		}
	}
}

void ARL_Arrow::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ARL_Arrow::Fire(FVector Direction,float Strength)
{
	// ���㷢�䷽�����ת
	FRotator ArrowRotation = Direction.Rotation();

	// ���ü�����ת��ʹ���泯���䷽��
	BoxCollision->SetRelativeRotation(ArrowRotation);

	// ���������˶����
	ProjectileMovement->Activate();

	// ���÷��䷽��,�ٶȺ���������
	ArrowVelocity = Direction * FMath::Lerp(MinSpeed, MaxSpeed, Strength);
	ProjectileMovement->Velocity = ArrowVelocity;
	ProjectileMovement->ProjectileGravityScale = FMath::Lerp(MaxGravity, MinGravity, Strength);

	//������ײ
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//��������
	if (ArrowShootSound && GetWorld())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ArrowShootSound, GetActorLocation());
	}

	//������Ч
	if (NiagaraEffect && NiagaraAttachComponent) // ȷ�� NiagaraEffect �� NiagaraAttachComponent ��Ч
	{
		ENCPoolMethod PoolingMethod = ENCPoolMethod::None; // ��ʼ�� ENCPoolMethod

		UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraEffect,                       // ���õ� Niagara ��Ч
			NiagaraAttachComponent,              // Ҫ���ӵ����
			NAME_None,                           // û���ض������ʹ�� NAME_None
			FVector::ZeroVector,                 // λ��ƫ��
			FRotator::ZeroRotator,               // ��ת
			FVector(1.0f),                       // ����
			EAttachLocation::KeepRelativeOffset, // ���ӹ��� (��ȷ��λ��)
			false,                               // �Ƿ��Զ�����
			PoolingMethod,                       // �ػ�����
			true                                 // �ֶ�������������
		);
	}
	//������������
	SetLifeSpan(10.0f);
}

//// Called every frame
//void ARL_Arrow::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

