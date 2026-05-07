// T24 - bNetLoadOnClient 与关卡 Actor 的复制
//
// 任务目标：理解关卡 Actor 在 Client 上的两种走向
//   bNetLoadOnClient = true（默认）  : Client 加载关卡时本地构造一份
//   bNetLoadOnClient = false         : Client 加载关卡时跳过该 Actor
//
// 与运行时 Spawn Actor 不同：
//   关卡 Actor    -> Client 加载就有，可被 Server 权威接管
//   Spawn Actor  -> Server 创建并复制到 Client
//
// 业务建议：
//   * 静态装饰物：默认即可
//   * 关卡里的"可破坏物 / 任务物"：bReplicates=true ，让 Server 销毁时 Client 跟随
//   * 想"Client 不要自己造一份的"：bNetLoadOnClient=false（少见）

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "T24_NetLoadOnClientActor.generated.h"

UCLASS()
class UETEST_API AT24_NetLoadOnClientActor : public AActor
{
	GENERATED_BODY()

public:
	AT24_NetLoadOnClientActor();
	virtual void BeginPlay() override;

	// 蓝图开关，便于在编辑器里改设置后比较行为
	UPROPERTY(EditAnywhere)
	bool T24_bDemoNetLoadFalse = false;
};
