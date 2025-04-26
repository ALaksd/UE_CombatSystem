#include "Component/RL_MovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/RL_InputBufferComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"
#include "GAS/ASC_Base.h"
#include "Input/RLInputComponent.h"
#include "Interface/RL_CharacterAimInterface.h"
#include "Item/Item_Pickup.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RL_PlayerState.h"
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
	if (InputSubsystem && BaseIMC)
	{
		InputSubsystem->AddMappingContext(BaseIMC, 0);
	}

	characterMovement = ownerCharacter->GetCharacterMovement();
	// 设置移动组件速度
	if (characterMovement)
	{
		characterMovement->MaxWalkSpeed = moveSpeed;
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
		//URLInventoryComponent* BackpComponent = Cast<URLInventoryComponent>(PlayerState->FindComponentByClass(URLInventoryComponent::StaticClass()));
		URLInventoryComponent_Equipment* EquipComponent = PlayerState->FindComponentByClass<URLInventoryComponent_Equipment>();

		if (EquipComponent)
		{
			EquipComponent->LootItem(ItemToPickup->ItemInstance);
		}
		// 销毁地上的物品
		ItemToPickup->Destroy();
	}
}

void URL_MovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ItemsCanPickup.Num()>0)
	{
		// 计算角色正前方30度范围内距离角色最近的一个可拾取物品
		FVector ForwardVector = ownerCharacter->GetActorForwardVector();
		for (AItem_Pickup* Item : ItemsCanPickup)
		{
			// 物品与玩家之间的向量
			FVector Dir = Item->GetActorLocation()-ownerCharacter->GetActorLocation();
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
		if (ItemToPickup)
			GEngine->AddOnScreenDebugMessage(-1, 0.8f, FColor::Red, FString::Printf(TEXT("The actor's name is: %s"), *ItemToPickup->GetName()));
		
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