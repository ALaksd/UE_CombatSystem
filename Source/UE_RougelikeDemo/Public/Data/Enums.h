#pragma once

#include "CoreMinimal.h"
#include "Enums.generated.h"

/*
 * 存储所有枚举
 */

// 通用技能
UENUM(BlueprintType)
enum class E_SkillType : uint8
{
	FireRant UMETA(DisplayName = "火焰附魔"),
	HealOverTime UMETA(DisplayName = "持续回血"),
	HealInstant UMETA(DisplayName = "单次回血"),
};

// 理智状态
UENUM(BlueprintType)  
enum class E_SanityState : uint8  
{  
	Sane        UMETA(DisplayName = "清醒"),  // 理智正常  
	Chaotic     UMETA(DisplayName = "混沌")   // 理智崩坏  
};  
