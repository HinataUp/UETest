// T19 - RPC 参数类型（值 / 结构体 / Actor指针 / TArray）
//
// 任务目标：把 RPC 参数走一遍，理解约束
//   * 引用参数 const T& 是函数签名优化，不影响网络协议
//   * Actor 指针参数 ：被引用 Actor 必须在接收端存在
//   * 大结构体 / 大数组 ：注意 MTU 限制（默认 1024B 上下）
//
// 反例：UObject* 一般无效（除 Actor / ReplicatedSubObject 外）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../T1_T5_PropertyReplication/T3_StructActor.h"  // 复用 FT3_CombatState
#include "T19_RPCParamsActor.generated.h"

UCLASS()
class UETEST_API AT19_RPCParamsActor : public AActor
{
	GENERATED_BODY()

public:
	AT19_RPCParamsActor();
	virtual void BeginPlay() override;

	// 一次性把多种参数演示完
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastComplex(
		int32 T19_IntVal,
		const FString& T19_StringVal,
		const FT3_CombatState& T19_StructVal,
		AActor* T19_ActorRef,
		const TArray<int32>& T19_ArrayVal);

private:
	FTimerHandle T19_TickTimer;
	void T19_ServerInvoke();
};
