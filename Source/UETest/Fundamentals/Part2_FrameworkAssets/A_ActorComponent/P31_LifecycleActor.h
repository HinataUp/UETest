// P31 - Actor 完整生命周期
//
// 顺序 ：Constructor → PostInitializeComponents → BeginPlay → Tick → EndPlay → Destroyed → GC
// 关键节点 ：
//   * Constructor 中 不能 访问 World（World 还没附着）
//   * PostInitializeComponents ：所有 DefaultSubobject 已就绪 ；可以访问 World
//   * BeginPlay ：所有 Actor 都进 PostInitializeComponents 之后 ；最安全的初始化点
//   * EndPlay ：Actor 即将销毁 ；用来清理 Timer / 解绑委托 / 释放资源

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P31_LifecycleActor.generated.h"

UCLASS()
class UETEST_API AP31_LifecycleActor : public AActor
{
	GENERATED_BODY()

public:
	AP31_LifecycleActor();

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual void Destroyed() override;

	// 单测可读 ：哪些回调被触发过（按位或）
	enum ELifecycleStage : int32
	{
		Stage_Ctor              = 1 << 0,
		Stage_PostInit          = 1 << 1,
		Stage_BeginPlay         = 1 << 2,
		Stage_Tick              = 1 << 3,
		Stage_EndPlay           = 1 << 4,
		Stage_Destroyed         = 1 << 5,
	};
	int32 StageFlags = 0;

private:
	// 默认 Tick 关闭 ；测试时可手动开
	void TouchStage(int32 Bit, const TCHAR* Name) const;
};
