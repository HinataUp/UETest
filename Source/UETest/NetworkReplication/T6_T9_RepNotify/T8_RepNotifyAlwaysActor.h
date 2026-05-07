// T8 - REPNOTIFY_Always vs REPNOTIFY_OnChanged，与"让 Authority 也走 OnRep"的标准模式
//
// 任务目标：
//   * REPNOTIFY_OnChanged（默认）：值真的变化时才触发 OnRep
//   * REPNOTIFY_Always           ：值即使未变，只要发生复制就触发 OnRep
//   * 标准模式：Server 改完属性后手动调用 OnRep_XXX(OldValue)，让两端走同一段表现代码
//
// 适用场景：被动技能"光环刷新"——即便数值没变，也想每次"光环重算"做一次 VFX 闪烁

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T8_RepNotifyAlwaysActor.generated.h"

UCLASS()
class UETEST_API AT8_RepNotifyAlwaysActor : public AActor
{
	GENERATED_BODY()

public:
	AT8_RepNotifyAlwaysActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Server 端业务函数：刷新光环（值可能不变）
	// 公开以便其他系统 / 测试直接调用
	void T8_ServerRefreshAura(int32 NewStacks);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_T8_AuraStacks, VisibleAnywhere)
	int32 T8_AuraStacks;

	UFUNCTION()
	void OnRep_T8_AuraStacks(int32 T8_OldStacks);

private:
	FTimerHandle T8_TickTimer;
	int32 T8_TickCount = 0;
	void T8_ServerTickAura();
};
