// T21 - Actor 复制的生成与销毁时序
//
// 任务目标：观察"Server Spawn / Destroy"在三端的时序
//   * Server BeginPlay 早于 Client BeginPlay（网络延迟）
//   * Client 端 BeginPlay 时，初始 Replicated 属性应已可用
//   * Destroy 必须在 Server 调用，Client 自动跟随
//
// 经验：在 Client 端 BeginPlay 里读 Replicated 字段是安全的——
// UE 保证初始属性同步到位后才在 Client 上调 BeginPlay 。

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T21_LifecycleTimingActor.generated.h"

UCLASS()
class UETEST_API AT21_LifecycleTimingActor : public AActor
{
	GENERATED_BODY()

public:
	AT21_LifecycleTimingActor();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 用于演示"Client BeginPlay 时初始值已就绪"
	UPROPERTY(Replicated)
	int32 T21_InitialValue;
};
