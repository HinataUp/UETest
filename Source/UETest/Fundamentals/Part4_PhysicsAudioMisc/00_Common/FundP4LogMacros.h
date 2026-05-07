// Part 4 - 物理 / 音频 / 特效 / 工程补充 公共日志宏

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFundP4, Log, All);

#define P4_LOG(Format, ...) \
	UE_LOG(LogFundP4, Warning, TEXT("[P4] " Format), ##__VA_ARGS__)
