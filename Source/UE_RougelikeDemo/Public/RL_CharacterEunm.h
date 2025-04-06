#pragma once
#include "CoreMinimal.h"

// 定义角色类型的枚举
UENUM(BlueprintType)  // 让枚举在蓝图中可用
enum class ECharacterType : uint8
{
	Warrior     UMETA(DisplayName = "Warrior"), //战士
	Caster      UMETA(DisplayName = "Caster"), //法师
	Archer      UMETA(DisplayName = "Archer"), //弓箭手
	Assassin    UMETA(DisplayName = "Assassin"),
	Healer      UMETA(DisplayName = "Healer")
};