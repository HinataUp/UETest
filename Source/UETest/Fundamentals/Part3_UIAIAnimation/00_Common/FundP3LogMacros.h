// Part 3 - UI / AI / 动画 公共日志宏

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFundP3, Log, All);

#define P3_LOG(Format, ...) \
	UE_LOG(LogFundP3, Warning, TEXT("[P3] " Format), ##__VA_ARGS__)
