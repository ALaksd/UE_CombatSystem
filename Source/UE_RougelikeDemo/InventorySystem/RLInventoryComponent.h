// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "RLInventoryComponent.generated.h"

class URLInventoryItemInstance;
/**
 * 存放物品的数据结构,暂时放在InventoryComponent
 */

USTRUCT(BlueprintType)
struct UE_ROUGELIKEDEMO_API FRLInventoryItemSlot
{
	GENERATED_BODY()

public:
	FRLInventoryItemSlot()
	:ItemInstance(nullptr),SlotId(0),SlotTags(FGameplayTagContainer::EmptyContainer)
	{

	}

	FRLInventoryItemSlot(const FRLInventoryItemSlot& OtherSlot)
	:ItemInstance(OtherSlot.ItemInstance),SlotId(OtherSlot.SlotId),SlotTags(OtherSlot.SlotTags)
	{

	}

	UPROPERTY(BlueprintReadWrite,VisibleInstanceOnly,Category = "Inventory")
	URLInventoryItemInstance* ItemInstance;	//ItemInstance指针，指向物品实例

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Inventory")
	int32 SlotId;	//SlotId为插槽的编号，一个背包内每个插槽的Id都是唯一的

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Inventory")
	FGameplayTagContainer SlotTags;

	bool operator==(const FRLInventoryItemSlot& Other) const { return this->SlotId == Other.SlotId && this->ItemInstance == Other.ItemInstance; }
	bool operator!=(const FRLInventoryItemSlot& Other) const { return !(FRLInventoryItemSlot::operator==(Other)); }

protected:
	TObjectPtr<URLInventoryComponent> Onwer;
	friend class URLInventoryComponent;

};

//存放插槽的数组
USTRUCT()
struct UE_ROUGELIKEDEMO_API FRLInventoryItemSlotArray
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FRLInventoryItemSlot> Slots;

	UPROPERTY()
	TObjectPtr<URLInventoryComponent> Onwer;
};

// FAnnaInventoryItemSlotHandle 结构体定义
USTRUCT(BlueprintType)
struct UE_ROUGELIKEDEMO_API FRLInventoryItemSlotHandle
{
	GENERATED_BODY()
public:
	// 默认构造函数
	FRLInventoryItemSlotHandle()
		: SlotId(-1), SlotTags(), ParentInventory(nullptr) {}

	// 拷贝构造函数
	FRLInventoryItemSlotHandle(const FRLInventoryItemSlotHandle& InHandle)
		: SlotId(InHandle.SlotId), SlotTags(InHandle.SlotTags), ParentInventory(InHandle.ParentInventory) {}

	// 从Slot构造
	FRLInventoryItemSlotHandle(const FRLInventoryItemSlot& FromSlot, URLInventoryComponent* InParentInventory)
		: SlotId(FromSlot.SlotId), SlotTags(FromSlot.SlotTags), ParentInventory(InParentInventory) {}

	// ID构造
	FRLInventoryItemSlotHandle(int32 InSlotId, URLInventoryComponent* InParentInventory)
		: SlotId(InSlotId), ParentInventory(InParentInventory) {}

	// Tag构造
	FRLInventoryItemSlotHandle(const FGameplayTag& InTag, URLInventoryComponent* InParentInventory)
		: SlotId(-1), SlotTags(FGameplayTagContainer(InTag)), ParentInventory(InParentInventory) {}

	// 运算符重载
	bool operator==(const FRLInventoryItemSlotHandle& Handle) const
	{
		//SlotId,Inventory,Tag全相同
		const bool bIdsMatch = (SlotId == Handle.SlotId);
		const bool bInventoryMatch = (ParentInventory == Handle.ParentInventory);
		const bool bTagMatch = SlotTags.HasAllExact(Handle.SlotTags);
		return bIdsMatch && bInventoryMatch && bTagMatch;
	}

	bool operator!=(const FRLInventoryItemSlotHandle& Handle) const
	{
		return !(*this == Handle);
	}

	bool operator==(const FRLInventoryItemSlot& Slot) const
	{
		const bool bIdsMatch = (SlotId == Slot.SlotId);
		const bool bTagMatch = SlotTags.HasAllExact(Slot.SlotTags);
		return bIdsMatch && bTagMatch;
	}

	bool operator!=(const FRLInventoryItemSlot& Slot) const
	{
		return !(*this == Slot);
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 SlotId;	//钥匙和它对应的柜子Id应该是一致的。这是用于根据Handle寻找对应的Slot的索引

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FGameplayTagContainer SlotTags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	URLInventoryComponent* ParentInventory;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnItemSlotUpdate,
	URLInventoryComponent*, InventoryComponent,
	const FRLInventoryItemSlotHandle&, SlotHandle,
	URLInventoryItemInstance*, NewItem,
	URLInventoryItemInstance*, OldItem);

/**
 * 背包系统需要增加，可堆叠物品的存放
 * 背包自动扩容
 * 
 */
UCLASS(BlueprintType,Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UE_ROUGELIKEDEMO_API URLInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URLInventoryComponent();

	/**
	 * 委托
	 */
	UPROPERTY(BlueprintAssignable,Category = "Inventory")
	FOnItemSlotUpdate OnItemSlotUpdate;

	/**
	 * 增删改查
	 */

	/** 找到背包中的第一个空的插槽，调用PlaceItemIntoSlot将物品实例放入插槽 */
	UFUNCTION(BlueprintCallable,Category = "Inventory")
	virtual bool LootItem(URLInventoryItemInstance* Item);

	/** 将物品放入指定的插槽当中 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool PlaceItemSlot(URLInventoryItemInstance* Item,const FRLInventoryItemSlotHandle& ItemHandle);

	/** 移除指定插槽 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool RemoveItemFromInventory(const FRLInventoryItemSlotHandle& SlotHandle);

	/** 移除全部插槽 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool RemoveAllItemsFromInventory(TArray<URLInventoryItemInstance*>& OutItemsRemoved);

	/** 返回当前背包所有的插槽钥匙 */
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Inventory")
	virtual TArray<FRLInventoryItemSlotHandle> GetAllSlotHandles();

	/** 获取指定插槽内存放的物品 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual URLInventoryItemInstance* GetItemInstanceInSlot(const FRLInventoryItemSlotHandle& Handle);

protected:
	virtual void BeginPlay() override;

	/** 创建插槽 */
	virtual void CreateInventorySlot(int32 Count = 1);
	/** 移除插槽 */
	virtual void RemoveInventorySlot(const FRLInventoryItemSlotHandle& Handle);

	/** PostInventoryUpdate函数会在当背包系统插槽发生变化调用，比如新增或移除插槽 */
	void PostInventoryUpdate();

	/** PopulateSlotReferenceArray用于对插槽重新排序 */
	void PopulateSlotReferenceArray(TArray<FRLInventoryItemSlotHandle>& Handles);

	/**
	* 工具函数
	 */

	 /** 用于判断SlotHandle - 插槽钥匙是否有效 */
	bool IsVaildItemSlot(const FRLInventoryItemSlotHandle& Handle);

	/** 输入SlotHandle，获取对应的Slot。根据钥匙，找到对应的柜子 */
	FRLInventoryItemSlot& GetItemSlot(const FRLInventoryItemSlotHandle& Handle);

	/** 判断指定的插槽Item Instance指针是否为空，如果为空则返回true，说明可以放入新的物品 */
	bool AcceptsItem(URLInventoryItemInstance* Item, const FRLInventoryItemSlotHandle& ItemHandle);

	/** 插槽数组和钥匙数组 */
	UPROPERTY()
	FRLInventoryItemSlotArray Inventory;

	TArray<FRLInventoryItemSlotHandle> AllSlotHandles;

	//插槽Id生成器，背包每生成一次插槽，计数器+1，确保所有插槽的Id都不相同
	int32 IdCounter;

	// 扩容增长因子（每次扩容增加当前容量的50%，类似std::vector）
	float CapacityGrowthFactor = 0.5f;
	// 最小扩容数量（保证每次至少增加这个数量）
	int MinGrowthAmount = 5;

};
