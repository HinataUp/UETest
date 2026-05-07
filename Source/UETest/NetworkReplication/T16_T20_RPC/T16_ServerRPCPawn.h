// T16 - Server RPC（Reliable vs Unreliable）
//
// 任务目标：让 Client 调用、Server 执行
// 关键约束：调用者必须是"Owner 是某 PlayerController"的 Actor —— Pawn 是天然合适的载体
//
// 三种典型 Server RPC 写法：
//   写法1：UFUNCTION(Server, Reliable)              ：保证到达，按顺序
//   写法2：UFUNCTION(Server, Unreliable)            ：高频可丢，省带宽
//   写法3：UFUNCTION(Server, Reliable, WithValidation) ：T20 演示

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "T16_ServerRPCPawn.generated.h"

UCLASS()
class UETEST_API AT16_ServerRPCPawn : public APawn
{
	GENERATED_BODY()

public:
	AT16_ServerRPCPawn();
	virtual void BeginPlay() override;

	// 写法1：Reliable Server RPC ：按顺序、不丢
	UFUNCTION(Server, Reliable)
	void ServerSendReliable(int32 T16_Value);

	// 写法2：Unreliable Server RPC ：高频小事件适用
	UFUNCTION(Server, Unreliable)
	void ServerSendUnreliable(int32 T16_Value);

private:
	// 用 Timer 模拟"Client 每秒按一下攻击键"
	FTimerHandle T16_TickTimer;
	int32 T16_LocalCounter = 0;

	void T16_ClientTickFire();
};
