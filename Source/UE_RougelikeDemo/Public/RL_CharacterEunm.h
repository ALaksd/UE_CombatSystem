#pragma once
#include "CoreMinimal.h"

// �����ɫ���͵�ö��
UENUM(BlueprintType)  // ��ö������ͼ�п���
enum class ECharacterType : uint8
{
	Warrior     UMETA(DisplayName = "Warrior"), //սʿ
	Caster      UMETA(DisplayName = "Caster"), //��ʦ
	Archer      UMETA(DisplayName = "Archer"), //������
	Assassin    UMETA(DisplayName = "Assassin"),
	Healer      UMETA(DisplayName = "Healer")
};