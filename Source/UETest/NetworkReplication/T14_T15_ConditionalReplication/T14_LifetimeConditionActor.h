// T14 - DOREPLIFETIME_CONDITION 五种常用条件
//
// 任务目标：精细控制"哪些字段同步给哪些 Connection"
// 五种 COND 与典型用途：
//   COND_OwnerOnly       ：只给 Owner 客户端 —— 玩家私有数据（背包 / 冷却）
//   COND_SkipOwner       ：除 Owner 外都同步 —— Owner 已本地处理过，不重复同步
//   COND_SimulatedOnly   ：只给"看别人"的端 —— 远端表现专用数据
//   COND_AutonomousOnly  ：只给"控制本 Pawn 的玩家"自己 —— 输入修正等
//   COND_InitialOnly     ：只在第一次到达时同步 —— 不变的初始化字段（Team ID）
//
// 注意：要测 OwnerOnly / SkipOwner / AutonomousOnly ，必须给 Actor SetOwner ，否则相当于全部屏蔽。
//        推荐放在玩家 Pawn / PlayerState 上跑这个 task 。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T14_LifetimeConditionActor.generated.h"

UCLASS()
class UETEST_API AT14_LifetimeConditionActor : public AActor
{
	GENERATED_BODY()

public:
	AT14_LifetimeConditionActor();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, VisibleAnywhere) int32 T14_RepOwnerOnly;
	UPROPERTY(Replicated, VisibleAnywhere) int32 T14_RepSkipOwner;
	UPROPERTY(Replicated, VisibleAnywhere) int32 T14_RepSimulatedOnly;
	UPROPERTY(Replicated, VisibleAnywhere) int32 T14_RepAutonomousOnly;
	UPROPERTY(Replicated, VisibleAnywhere) int32 T14_RepInitialOnly;

private:
	FTimerHandle T14_TickTimer;
	void T14_ServerTickAll();
};
