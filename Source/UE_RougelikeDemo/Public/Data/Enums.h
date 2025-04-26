#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

/*
 * 存储所有枚举
 */

UENUM(BlueprintType)
enum class E_SkillType : uint8
{
	FireRant UMETA(DisplayName = "火焰附魔"),
	HealOverTime UMETA(DisplayName = "持续回血"),
	HealInstant UMETA(DisplayName = "单次回血"),
};