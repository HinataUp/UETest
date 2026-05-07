// Part 2 - Gameplay 框架 + 资产系统 + 子系统 公共日志宏

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFundP2, Log, All);

#define P2_LOG(Format, ...) \
	UE_LOG(LogFundP2, Warning, TEXT("[P2] " Format), ##__VA_ARGS__)
