#include "Component/RL_MovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Enemy_Base.h"
#include "Character/RL_BaseCharacter.h"
#include "Component/RL_InputBufferComponent.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"
#include "GAS/ASC_Base.h"
#include "Input/RLInputComponent.h"
#include "InteractableActor/Interactable_Base.h"
#include "Interface/RL_CharacterAimInterface.h"
#include "Item/Item_Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/RL_PlayerState.h"
#include "UE_RougelikeDemo/UE_RougelikeDemo.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryComponent.h"
#include "UE_RougelikeDemo/InventorySystem/InventoryComponent/RLInventoryComponent_Equipment.h"

URL_MovementComponent::URL_MovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URL_MovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// 获取角色和控制器
	ownerCharacter = Cast<ACharacter>(GetOwner());
	if (!ownerCharacter) return;
	playerController = Cast<APlayerController>(ownerCharacter->GetController());
	if (!playerController) return;
	// 将输入映射上下文添加到本地玩家子系统
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	if (InputSubsystem && BaseIMC && MoveIMC)
	{
		InputSubsystem->AddMappingContext(BaseIMC, 0);
		InputSubsystem->AddMappingContext(MoveIMC, 1);
	}

	characterMovement = ownerCharacter->GetCharacterMovement();
	// 设置移动组件速度
	if (characterMovement)
	{
		UpdateMovementState(EMovementState::Jogging);
		characterMovement->bOrientRotationToMovement = true; // 是否朝向移动方向
		characterMovement->bUseControllerDesiredRotation = false; // 允许控制器控制旋转
		ownerCharacter->bUseControllerRotationYaw = false;
	}

	// 绑定输入动作
	if (URLInputComponent* RLInputComponent = CastChecked<URLInputComponent>(ownerCharacter->InputComponent))
	{
		RLInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &URL_MovementComponent::Move);

		RLInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &URL_MovementComponent::Look);

		RLInputComponent->BindAction(CollectAction, ETriggerEvent::Started, this, &URL_MovementComponent::Collect);

		RLInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &URL_MovementComponent::Run);

		RLInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &URL_MovementComponent::RunOver);

		RLInputComponent->BindAction(LockAction, ETriggerEvent::Triggered, this, &URL_MovementComponent::ToggleLockOn);

		RLInputComponent->BindAction(STLAction, ETriggerEvent::Triggered, this, &URL_MovementComponent::SwitchTargetLeft);

		RLInputComponent->BindAction(STRAction, ETriggerEvent::Triggered, this, &URL_MovementComponent::SwitchTargetRight);
		// 处决
		RLInputComponent->BindAction(ExecuteAction, ETriggerEvent::Triggered, this, &URL_MovementComponent::Execute);

		// 武器切换
		if (ARL_PlayerState* PlayerState = CastChecked<ARL_PlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0)))
			if (URLInventoryComponent_Equipment* Equipment = CastChecked<URLInventoryComponent_Equipment>(PlayerState->EquipmentInventoryComponnet))
				RLInputComponent->BindAction(SwitchWeaponAction,ETriggerEvent::Started, Equipment, &URLInventoryComponent_Equipment::SwitchWeapon);
		
		RLInputComponent->BindAbilityInputAction(InputConfig,this,&ThisClass::LMBInputPressedTest,&ThisClass::LMBInputReleasedTest,&ThisClass::LMBInputHeldTest);
	}
}

void URL_MovementComponent::Move(const FInputActionValue& Value)
{
	if (!ownerCharacter || !playerController) return;
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator ControlRotation = playerController->GetControlRotation();
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	ownerCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	ownerCharacter->AddMovementInput(RightDirection, MovementVector.X);
}

void URL_MovementComponent::Look(const FInputActionValue& Value)
{
	if (!playerController) return;

	FVector2D LookAxis = Value.Get<FVector2D>();
	playerController->AddYawInput(LookAxis.X);
	playerController->AddPitchInput(LookAxis.Y);
}

void URL_MovementComponent::Run(const FInputActionValue& Value)
{
	UAbilitySystemComponent* ASC = Cast<ARL_BaseCharacter>(ownerCharacter)->GetAbilitySystemComponent();
	ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(FName("Ability.Run"))));
}   

void URL_MovementComponent::RunOver(const FInputActionValue& Value)
{
	UAbilitySystemComponent* ASC = Cast<ARL_BaseCharacter>(ownerCharacter)->GetAbilitySystemComponent();
	UASC_Base* ASCB = Cast<UASC_Base>(ASC);
	ASCB->AbilityInputTagReleased(FGameplayTag::RequestGameplayTag(FName("Ability.Run")));
}

void URL_MovementComponent::UpdateMovementState(EMovementState State)
{
	FMovementSetting* MovementSettingPtr = MovementSettingMap.Find(State);
	if(MovementSettingPtr)
	{
		CurrentMovementState = State;
		ownerCharacter->GetCharacterMovement()->MaxWalkSpeed = MovementSettingPtr->MaxWalkSpeed;
		ownerCharacter->GetCharacterMovement()->MaxAcceleration = MovementSettingPtr->MaxAcceleration;
		ownerCharacter->GetCharacterMovement()->BrakingDecelerationWalking = MovementSettingPtr->BrakingDeceleration;
		ownerCharacter->GetCharacterMovement()->BrakingFrictionFactor = MovementSettingPtr->BrakingFrictionFactor;
		ownerCharacter->GetCharacterMovement()->BrakingFriction = MovementSettingPtr->BrakingFriction;
		ownerCharacter->GetCharacterMovement()->bUseSeparateBrakingFriction = MovementSettingPtr->bUseSeparateBrakingFriction;

		if (ownerCharacter->GetMesh()->GetAnimInstance()->Implements<URL_CharacterAimInterface>())
		{
			IRL_CharacterAimInterface::Execute_ReciveMovementState(ownerCharacter->GetMesh()->GetAnimInstance(),State);
		}
	}
}

void URL_MovementComponent::Collect(const FInputActionValue& Value)
{
	if (ItemToPickup)
	{
		// 物品加入背包
		ARL_PlayerState* PlayerState = Cast<ARL_PlayerState>(UGameplayStatics::GetPlayerState(GetWorld(),0));

		//这里先暂时加入装备背包
		URLInventoryComponent* BackpComponent = Cast<URLInventoryComponent>(PlayerState->FindComponentByClass(URLInventoryComponent::StaticClass()));
		//URLInventoryComponent_Equipment* EquipComponent = PlayerState->FindComponentByClass<URLInventoryComponent_Equipment>();

		if (BackpComponent)
		{
			BackpComponent->LootItem(ItemToPickup->ItemInstance);
			// 销毁地上的物品
			ItemToPickup->Destroy();
		}
	}
	else
	{
		if (InteractableActor)
		{
			InteractableActor->TryInteract();
		}
	}
}

void URL_MovementComponent::Execute(const FInputActionValue& Value)
{
	// 看有无锁定的敌人
	if (CurrentTarget == nullptr) return;
	AEnemy_Base* Enemy = Cast<AEnemy_Base>(CurrentTarget);
	if (Enemy == nullptr || ownerCharacter == nullptr) return;

	FVector Direction = ownerCharacter->GetActorLocation() - Enemy->GetActorLocation();
	FVector EnemyDir = Enemy->GetActorForwardVector();
	float Distance = Direction.Length();
	
	// 正面判定区域
	float Angle = CalculateAngleBetweenVectors(Direction,EnemyDir);
	// 背面判定区域
	float BackAngle = CalculateAngleBetweenVectors(Direction,-EnemyDir);

	// 处决条件
	/*
	 * 正面处决
	 *
	 * 玩家处在敌人正面
	 * 敌人处于破防状态
	 * 距离合适
	 */
	if (ExecuteAngle >= Angle && Distance <= ExecuteDistance && Enemy->bIsGuardBroken)
	{
		// 角度与距离判定   敌人破防状态判定
		// 触发处决
		/*
		 * 调整玩家到合适位置
		 * 玩家处决GA
		 * 敌人播放处决动画
		 */

		FVector TargetLocation = Enemy->GetMesh()->GetSocketLocation(FName("Socket_Execute_F"));
		// 计算目标旋转（面向敌人）
		FRotator TargetRotation = Enemy->GetActorRotation() + FRotator(0, 180.0f, 0);
		FHitResult SweepHitResult;
		// 设置位置旋转
		bool bSweep = ownerCharacter->SetActorLocationAndRotation(TargetLocation,TargetRotation,true,&SweepHitResult,ETeleportType::TeleportPhysics);
		if (!bSweep)
		{
			UE_LOG(LogTemp,Warning,TEXT("发生碰撞,处决取消"));
		}
		else
		{
			FGameplayTag ExecuteTag = FGameplayTag::RequestGameplayTag(FName("InputTag.FrontExecution"));
			LMBInputHeldTest(ExecuteTag);
			Enemy->Execute(true);
		}
	}

	
	

	/*
	 * 背面处决
	 *
	 * 双方不在战斗状态
	 * 敌人在玩家一定距离内
	 * 角度合适
	 */

	if (ExecuteAngle >= Angle && Distance <= ExecuteDistance && !Enemy->bIsFindPlayer)
	{
		FVector TargetLocation = Enemy->GetMesh()->GetSocketLocation(FName("Socket_Execute_B"));
		// 计算目标旋转（面向敌人背部）
		FRotator TargetRotation = Enemy->GetActorRotation();
		FHitResult SweepHitResult;
		// 设置位置旋转
		bool bSweep = ownerCharacter->SetActorLocationAndRotation(TargetLocation,TargetRotation,true,&SweepHitResult,ETeleportType::TeleportPhysics);
		if (!bSweep)
		{
			UE_LOG(LogTemp,Warning,TEXT("发生碰撞,处决取消"));
		}
		else
		{
			FGameplayTag ExecuteTag = FGameplayTag::RequestGameplayTag(FName("InputTag.RearExecution"));
			LMBInputHeldTest(ExecuteTag);
			Enemy->Execute(false);
		}
	}
	
}

void URL_MovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateLockOnRotation(DeltaTime);

	if (ItemsCanPickup.Num()>0)
	{
		// 计算角色正前方40度范围内距离角色最近的一个可拾取物品
		FVector ForwardVector = ownerCharacter->GetActorForwardVector();
		for (AItem_Pickup* Item : ItemsCanPickup)
		{
			// 物品与玩家之间的向量
			FVector Dir = Item->GetActorLocation()-ownerCharacter->GetActorLocation();
			Dir.Z=0;
			ForwardVector.Z=0;
			
			float Angle = CalculateAngleBetweenVectors(ForwardVector,Dir);
			if (Angle<40)
			{
				// 计算离玩家最近的Actor
				if (ItemToPickup)
				{
					// 当前可拾取物品与玩家之间的向量 
					FVector Dis = ItemToPickup->GetActorLocation()-ownerCharacter->GetActorLocation();
					if (Dis.Length()>Dir.Length())
					{
						ItemToPickup=Item;
					}
				}
				else
					ItemToPickup=Item;
			}
		}

		// TODO:将可拾取物品相关信息显示
		// if (ItemToPickup)
		// 	;
	}
	else
	{
		ItemToPickup=nullptr;
	}
}

void URL_MovementComponent::AddItemCanPickup(AItem_Pickup* ItemToPickup_T)
{
	ItemsCanPickup.Add(ItemToPickup_T);
}

void URL_MovementComponent::RemoveItemCanPickup(AItem_Pickup* ItemToPickup_T)
{
	ItemsCanPickup.Remove(ItemToPickup_T);
}

float URL_MovementComponent::CalculateAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB)
{
	// 计算两个向量之间的夹角
	// 归一化向量（确保是单位向量）
	FVector NormalizedA = VectorA.GetSafeNormal();
	FVector NormalizedB = VectorB.GetSafeNormal();
	
	// 计算点积
	float DotProduct = FVector::DotProduct(NormalizedA, NormalizedB);

	// 处理浮点精度问题（确保点积在 [-1, 1] 范围内）
	DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);

	// 计算反余弦得到弧度值
	float AngleRadians = FMath::Acos(DotProduct);
	// 计算角度值
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	return AngleDegrees;
}

void URL_MovementComponent::AddInteractableActor(AInteractable_Base* InteractableActor_T)
{
	InteractableActor=InteractableActor_T;
}

void URL_MovementComponent::RemoveInteractableActor()
{
	InteractableActor=nullptr;
}

void URL_MovementComponent::LMBInputPressedTest(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Pressed")));
}

void URL_MovementComponent::LMBInputHeldTest(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Blue, FString::Printf(TEXT("Held")));
	CastChecked<UASC_Base>(ownerCharacter->GetPlayerState()->FindComponentByClass<UAbilitySystemComponent>())->AbilityInputTagHeld(InputTag);

	//缓存预输入
	ownerCharacter->FindComponentByClass<URL_InputBufferComponent>()->BufferInput(InputTag);
}

void URL_MovementComponent::LMBInputReleasedTest(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Green, FString::Printf(TEXT("Released")));
	CastChecked<UASC_Base>(ownerCharacter->GetPlayerState()->FindComponentByClass<UAbilitySystemComponent>())->AbilityInputTagReleased(InputTag);
}

void URL_MovementComponent::ToggleLockOn()
{
	if (ownerCharacter->Tags.Contains(PlayerLockingTag))
	{
		ownerCharacter->Tags.Remove(PlayerLockingTag);
		characterMovement->bOrientRotationToMovement = true; // 是否朝向移动方向
		characterMovement->bUseControllerDesiredRotation = false; // 允许控制器控制旋转
		CurrentTarget = nullptr;
	}
	else
	{
		characterMovement->bOrientRotationToMovement = false; // 是否朝向移动方向
		characterMovement->bUseControllerDesiredRotation = true; // 允许控制器控制旋转
		FindLockOnTarget();
	}
}

void URL_MovementComponent::FindLockOnTarget()
{
	if (!ownerCharacter || !playerController) return;
	
	int32 ViewportX, ViewportY;
	playerController->GetViewportSize(ViewportX, ViewportY);
	FVector2D ScreenCenter(ViewportX * 0.5f, ViewportY * 0.5f);

	LockableTargets.Empty();
	TArray<TPair<AActor*, float>> TargetScreenDistances;
	
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(ownerCharacter);

	float SearchRadius = 2000.0f;
	bool bHit = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		ownerCharacter->GetActorLocation(),
		FQuat::Identity,
		ECC_Enemy,
		FCollisionShape::MakeSphere(SearchRadius),
		Params
	);

	if (bHit)
	{
		for (auto& Result : Overlaps)
		{
			AActor* Target = Result.GetActor();
			if (!Target) continue;
			
			if (!Target->Tags.Contains(LockableTag)) continue;			

			FVector2D ScreenPos;
			bool bIsOnScreen = playerController->ProjectWorldLocationToScreen(Target->GetActorLocation(), ScreenPos);

			if (bIsOnScreen &&
				ScreenPos.X >= 0 && ScreenPos.X <= ViewportX &&
				ScreenPos.Y >= 0 && ScreenPos.Y <= ViewportY)
			{
				float DistanceToCenter = FVector2D::Distance(ScreenPos, ScreenCenter);
				TargetScreenDistances.Add(TPair<AActor*, float>(Target, DistanceToCenter));
			}
		}
	}
	
	if (TargetScreenDistances.Num() > 0)
	{
		TargetScreenDistances.Sort([](const TPair<AActor*, float>& A, const TPair<AActor*, float>& B)
		{
			return A.Value < B.Value;
		});

		for (auto& Pair : TargetScreenDistances)
		{
			LockableTargets.Add(Pair.Key);
		}

		CurrentTargetIndex = 0;
		CurrentTarget = LockableTargets[CurrentTargetIndex];
		
		if (!ownerCharacter->Tags.Contains(PlayerLockingTag))
		{
			ownerCharacter->Tags.Add(PlayerLockingTag);
		}
	}
	else
	{
		CurrentTarget = nullptr;
		if (ownerCharacter->Tags.Contains(PlayerLockingTag))
		{
			ownerCharacter->Tags.Remove(PlayerLockingTag);
		}
	}
}

void URL_MovementComponent::UpdateLockOnRotation(float DeltaTime)
{
	if (!ownerCharacter->Tags.Contains(PlayerLockingTag) || !CurrentTarget) return;

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(ownerCharacter->GetActorLocation(), CurrentTarget->GetActorLocation());
	FRotator TargetRotation(0.f, LookAtRotation.Yaw, 0.f);

	FRotator NewRotation = FMath::RInterpTo(ownerCharacter->GetActorRotation(), TargetRotation, DeltaTime, 10.f);
	ownerCharacter->SetActorRotation(NewRotation);
	
	AController* Controller = ownerCharacter->GetController();
	if (Controller)
	{
		Controller->SetControlRotation(TargetRotation);
	}
}

void URL_MovementComponent::SwitchTargetLeft()
{
	if (!ownerCharacter->Tags.Contains(PlayerLockingTag) || LockableTargets.Num() <= 1) return;

	CurrentTargetIndex = (CurrentTargetIndex - 1 + LockableTargets.Num()) % LockableTargets.Num();
	CurrentTarget = LockableTargets[CurrentTargetIndex];
}

void URL_MovementComponent::SwitchTargetRight()
{
	if (!ownerCharacter->Tags.Contains(PlayerLockingTag) || LockableTargets.Num() <= 1) return;

	CurrentTargetIndex = (CurrentTargetIndex + 1) % LockableTargets.Num();
	CurrentTarget = LockableTargets[CurrentTargetIndex];
}