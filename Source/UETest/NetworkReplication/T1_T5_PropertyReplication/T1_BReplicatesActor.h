// T1 - bReplicates 开关 与 Replicated 字段标记
//
// 任务目标：理解两个独立开关 ——
//   1) Actor.bReplicates  ：决定 Actor 是否参与复制
//   2) UPROPERTY(Replicated)：决定字段是否复制
// 两个开关都开，字段才能在 Client 上跟随变化。
//
// 常见写法：
//   写法A：构造函数 bReplicates = true   （推荐，最稳定）
//   写法B：运行时 SetReplicates(true)    （动态启停时用）
//   写法C：BP 中勾选 Replicates           （蓝图同等效果）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T1_BReplicatesActor.generated.h"

UCLASS()
class UETEST_API AT1_BReplicatesActor : public AActor
{
	GENERATED_BODY()

public:
	AT1_BReplicatesActor();

	virtual void BeginPlay() override;
	// 必须 override 才能让属性复制系统知道哪些字段要复制
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// 字段加 T1_ 前缀，避免与其他 task 命名冲突
	// 写法说明：UPROPERTY(Replicated) 表明该字段参与复制；
	// 实际复制规则需要在 GetLifetimeReplicatedProps 里再次声明（DOREPLIFETIME 宏）
	UPROPERTY(Replicated, VisibleAnywhere)
	int32 T1_TestValue;

	// 用于演示对比：若取消 Replicated 标记，Client 永远看不到变化
	UPROPERTY(VisibleAnywhere)
	int32 T1_NotReplicatedValue;

private:
	// Server 用的 Timer 句柄
	FTimerHandle T1_TickTimer;

	// 仅 Server 调用：每秒 ++ TestValue
	void T1_ServerTickValue();
};
