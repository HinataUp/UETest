// GAS 模块公共日志宏
// 复用网络同步模块的 NET_LOG 风格，但分类标签为 [GAS]，便于过滤

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGASTest, Log, All);

// 业务日志：自动带 GAS 前缀，Warning 级方便 Output Log 颜色区分
#define GAS_LOG(Format, ...) \
	UE_LOG(LogGASTest, Warning, TEXT("[GAS] " Format), ##__VA_ARGS__)

// 详细日志：默认关闭，必要时用 `log LogGASTest Verbose` 打开
#define GAS_VLOG(Format, ...) \
	UE_LOG(LogGASTest, Verbose, TEXT("[GAS] " Format), ##__VA_ARGS__)
