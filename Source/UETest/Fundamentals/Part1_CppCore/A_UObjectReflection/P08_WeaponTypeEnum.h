// P08 - UENUM ：BP 可见枚举与 UMETA 显示名
//
// 关键约束 ：必须 ": uint8" 才能参与反射 / BP / 复制
// UMETA(DisplayName="...") 在 BP 下拉菜单显示中文 / 自定义名
// UMETA(Hidden) 隐藏菜单选项（如 MAX 哨兵值）

#pragma once

#include "CoreMinimal.h"
#include "P08_WeaponTypeEnum.generated.h"

UENUM(BlueprintType)
enum class EP08_WeaponType : uint8
{
	None    UMETA(DisplayName = "无"),
	Sword   UMETA(DisplayName = "单手剑"),
	Axe     UMETA(DisplayName = "斧头"),
	Bow     UMETA(DisplayName = "弓箭"),
	MAX     UMETA(Hidden)
};
