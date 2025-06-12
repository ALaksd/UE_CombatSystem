#include "Component/ArmorComponent.h"

#include "Armor/ArmorBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemDefinition.h"
#include "UE_RougelikeDemo/InventorySystem/RLInventoryItemInstance.h"
#include "UE_RougelikeDemo/InventorySystem/Fragments/RLItemFragment_Attached.h"
#include "UE_RougelikeDemo/InventorySystem/InventoryComponent/RLInventoryComponent_Equipment.h"

UArmorComponent::UArmorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UArmorComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());

	EquipmentInventoryComponent = Cast<APawn>(Owner)->GetPlayerState()->FindComponentByClass<URLInventoryComponent_Equipment>();

	check(EquipmentInventoryComponent);

	EquipmentInventoryComponent->OnArmorUpdate.AddDynamic(this, &UArmorComponent::SwitchArmor);

	InitArmor();
}

void UArmorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UArmorComponent::SwitchArmor(URLInventoryItemInstance* NewArmor, const FRLInventoryItemSlotHandle& SlotHandle)
{
	if (CurrentArmor && SlotHandle.SlotTags.HasTag(FGameplayTag::RequestGameplayTag("Item.Armor.Body.Slot")))
	{
		const URLItemFragment_Attached* Fragment = Cast<URLItemFragment_Attached>(CurrentArmor->GetItemDefinition()->FindFragmentByClass(URLItemFragment_Attached::StaticClass()));
		Fragment->RemoveArmor(Owner);
		InitArmor();
	}

	if (CurrentHelmet && SlotHandle.SlotTags.HasTag(FGameplayTag::RequestGameplayTag("Item.Armor.Head.Slot")))
	{
		const URLItemFragment_Attached* Fragment = Cast<URLItemFragment_Attached>(CurrentHelmet->GetItemDefinition()->FindFragmentByClass(URLItemFragment_Attached::StaticClass()));
		Fragment->RemoveArmor(Owner);
	}
	
	if (!NewArmor)
		return;
	
	// 创建新实例
	const URLItemFragment_Attached* Fragment = Cast<URLItemFragment_Attached>(
	NewArmor->GetItemDefinition()->FindFragmentByClass(URLItemFragment_Attached::StaticClass()));

	if (Fragment)
	{
		URLInventoryItemInstance* Armor = Fragment->ArmorAttachToActor(GetOwner(), NewArmor);
		if (Armor && Armor->GetItemDefinition()->ItemTags.Added.HasTag(FGameplayTag::RequestGameplayTag("Item.Armor.Body")))
		{
			CurrentArmor = Armor;
		}
		else if (Armor && Armor->GetItemDefinition()->ItemTags.Added.HasTag(FGameplayTag::RequestGameplayTag("Item.Armor.Head")))
		{
			CurrentHelmet = Armor;
		}
	}	
}

void UArmorComponent::InitArmor()
{
	AActor* AttachedActor = nullptr;
	USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(Owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	
	// 检查是否指定了正确的 AttachedActorClass
	for (auto Pair : InitialArmor)
	{
		AttachedActor = Owner->GetWorld()->SpawnActor<AActor>(Pair.Value, Owner->GetActorLocation(), FRotator::ZeroRotator);
		// 尝试获取目标插槽，附加到物体上
		if (SkeletalMeshComp && !Pair.Key.IsNone() && AttachedActor)
		{
			// 使用 AttachToComponent 将附加的 Actor 绑定到插槽上
			EAttachmentRule LocationRule = EAttachmentRule::SnapToTarget;
			EAttachmentRule RotationRule = EAttachmentRule::SnapToTarget;
			EAttachmentRule ScaleRule = EAttachmentRule::KeepRelative;
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules(LocationRule, RotationRule, ScaleRule, true);
			AttachedActor->AttachToComponent(SkeletalMeshComp, AttachmentTransformRules, Pair.Key);

			AttachedActor->SetActorRelativeLocation(FVector::ZeroVector);
			AttachedActor->SetActorRelativeRotation(FRotator::ZeroRotator);

			//设置主骨骼
			AArmorBase* Armor = Cast<AArmorBase>(AttachedActor);
			if (Armor)
			{
				Armor->Bone->SetLeaderPoseComponent(SkeletalMeshComp);
			}
		}
	}
}

