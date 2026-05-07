// 渲染模块公共日志宏

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogRenderTest, Log, All);

#define RT_LOG(Format, ...) \
	UE_LOG(LogRenderTest, Warning, TEXT("[Render] " Format), ##__VA_ARGS__)
