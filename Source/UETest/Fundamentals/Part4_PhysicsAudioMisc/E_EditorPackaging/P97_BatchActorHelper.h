// P97 - Editor Utility Widget 批量操作 ：BP 友好接口
//
// 完整的"Editor Utility Widget" 需要 UnrealEd / Blutility 模块依赖（仅编辑器构建）。
// 本 Helper 提供运行时可调的批量操作接口 ；
// 编辑器中可在 EUW 蓝图中调用本 Library 实现"批量重命名 / 批量改 Tag"。
//
// 完整 EUW 步骤（README 中描述）：
//   1) Project → Edit → Plugins → 启用 "Editor Scripting Utilities"
//   2) Content Browser → Editor Utilities → Editor Utility Widget
//   3) 在 EUW 蓝图里加按钮 ：调本 Helper 的 BlueprintCallable

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P97_BatchActorHelper.generated.h"

UCLASS()
class UETEST_API UP97_BatchActorHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 批量给 Actors 加 Tag
	UFUNCTION(BlueprintCallable, Category = "P97|Editor")
	static int32 AddTagToActors(const TArray<AActor*>& Actors, FName Tag);

	// 批量按 Prefix + Index 重命名 Actor 标签（Actor->Tags 内）
	// 注意 ：不修改 Actor 名（FName 名通常由 Outer / 蓝图实例管理）
	UFUNCTION(BlueprintCallable, Category = "P97|Editor")
	static int32 PrefixActorTags(const TArray<AActor*>& Actors, FName Prefix);

	// 批量启 / 停 物理模拟（StaticMeshComponent）
	UFUNCTION(BlueprintCallable, Category = "P97|Editor")
	static int32 ToggleSimulatePhysics(const TArray<AActor*>& Actors, bool bEnable);
};
