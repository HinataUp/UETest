// Part 1 - C++ 核心与对象模型 公共日志宏

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFundP1, Log, All);

#define P1_LOG(Format, ...) \
	UE_LOG(LogFundP1, Warning, TEXT("[P1] " Format), ##__VA_ARGS__)
