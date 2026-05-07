// T18 - Multicast RPC（NetMulticast）
//
// 任务目标：Server 调用，所有"相关"Client 都执行；Server 自己也会执行一遍
// 强烈建议 Unreliable —— 群体广播 + Reliable 极易撑爆带宽
//
// 典型场景：
//   * 爆炸 / 命中特效广播
//   * 群体震屏 / 提示
//   GAS 中 GameplayCue 即是 Multicast 的优化封装

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T18_MulticastRPCActor.generated.h"

UCLASS()
class UETEST_API AT18_MulticastRPCActor : public AActor
{
	GENERATED_BODY()

public:
	AT18_MulticastRPCActor();
	virtual void BeginPlay() override;

	// Multicast RPC 写法：
	// 注意：Multicast 必须在 Authority 端调用；Client 调用会失败
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayVFX(FVector T18_Location);

private:
	FTimerHandle T18_TickTimer;
	void T18_ServerBroadcastVFX();
};
