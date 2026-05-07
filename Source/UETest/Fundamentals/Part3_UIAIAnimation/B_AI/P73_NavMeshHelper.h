// P73 - NavMesh + PathFollowing
//
// 编辑器配置 ：放 NavMeshBoundsVolume → Build Paths
// 代码端 ：AIController->MoveToActor / MoveToLocation

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P73_NavMeshHelper.generated.h"

class AAIController;

UCLASS()
class UETEST_API UP73_NavMeshHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 让 AI 移动到目标 ；返回是否成功发起请求
	UFUNCTION(BlueprintCallable, Category = "P73|Nav")
	static bool MoveAIToActor(AAIController* AICon, AActor* Target, float AcceptanceRadius = 50.f);

	// 让 AI 走到指定坐标
	UFUNCTION(BlueprintCallable, Category = "P73|Nav")
	static bool MoveAIToLocation(AAIController* AICon, FVector Location, float AcceptanceRadius = 50.f);

	// 在导航网格上随机找一点（用 GetRandomPointInNavigableRadius）
	UFUNCTION(BlueprintCallable, Category = "P73|Nav", meta = (WorldContext = "WorldContext"))
	static bool GetRandomNavLocation(const UObject* WorldContext, FVector Origin, float Radius, FVector& OutLocation);
};
