// T7 - RepNotify 带 OldValue 参数
//
// 任务目标：在 OnRep 里同时拿到"旧值"和"新值"，决策更精确
//   例：HP 从 100 降到 50 时播受伤反馈，从 50 升到 100 时不播
//
// 关键点：OldValue 是 "Client 端本地上次复制到的值"，不是 Server 的历史
// 函数签名规范：
//   * UFUNCTION() void OnRep_X(T OldValue);              （值类型）
//   * UFUNCTION() void OnRep_X(const T& OldValue);       （引用，避免大结构体拷贝）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T7_OnRepWithOldValueActor.generated.h"

UCLASS()
class UETEST_API AT7_OnRepWithOldValueActor : public AActor
{
	GENERATED_BODY()

public:
	AT7_OnRepWithOldValueActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_T7_Health, VisibleAnywhere)
	int32 T7_Health;

	// 带 OldValue 的 OnRep —— 签名必须严格匹配，否则 UE 不会传递 OldValue
	UFUNCTION()
	void OnRep_T7_Health(int32 T7_OldHealth);

private:
	FTimerHandle T7_TickTimer;
	int32 T7_TickCount = 0;
	void T7_ServerTickHealth();
};
