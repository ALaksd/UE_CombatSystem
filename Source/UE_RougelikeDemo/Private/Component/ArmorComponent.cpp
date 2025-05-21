#include "Component/ArmorComponent.h"

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
}

void UArmorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UArmorComponent::SwitchArmor(URLInventoryItemInstance* NewArmor)
{
	// 销毁之前的武器
	if (CurrentArmor)
		CurrentArmor->Destroy();
	
	if (!NewArmor)
		return;
	
	if (NewArmor->GetItemDefinition()->ItemTags.Added.HasTag(FGameplayTag::RequestGameplayTag("Item.Armor")))
	{
		// 创建新实例
		const URLItemFragment_Attached* Fragment = Cast<URLItemFragment_Attached>(
			NewArmor->GetItemDefinition()->FindFragmentByClass(URLItemFragment_Attached::StaticClass()));
	

		if (Fragment)
		{
			if (AActor* NewArmor_T = Fragment->AttachToActor(GetOwner(),NewArmor))
			{
			
				NewArmor_T->SetActorHiddenInGame(false);

				CurrentArmor = NewArmor_T;
			}
		}	
	}
}

