// T17 - Client RPC（Server 调用，特定 Client 执行）
//
// 任务目标：Server 主动通知"被打的那一个玩家"
// 关键点：Client RPC 走 Owner Connection ，所以 Actor 必须有 Owner
// 反例：在没有 Owner 的场景 Actor 上调 Client RPC ，会被静默忽略并打 warning

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "T17_ClientRPCPawn.generated.h"

UCLASS()
class UETEST_API AT17_ClientRPCPawn : public APawn
{
	GENERATED_BODY()

public:
	AT17_ClientRPCPawn();
	virtual void BeginPlay() override;

	// Client RPC ：Server 调用 -> Owner 客户端执行
	// 多用于"私有反馈"：屏幕闪红 / 个人提示 / 私聊
	UFUNCTION(Client, Reliable)
	void ClientShowMessage(const FString& T17_Msg);

private:
	FTimerHandle T17_TickTimer;
	void T17_ServerTickNotifyOwner();
};
