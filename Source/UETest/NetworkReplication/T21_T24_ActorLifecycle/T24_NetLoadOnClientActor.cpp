// T24 - bNetLoadOnClient 实现

#include "T24_NetLoadOnClientActor.h"
#include "../Common/NetTestLogMacros.h"

AT24_NetLoadOnClientActor::AT24_NetLoadOnClientActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// 关卡 Actor 是否需要被 Server 权威接管？是 -> bReplicates=true
	bReplicates = true;

	// 写法1（默认）：Client 加载关卡时也会构造此 Actor
	bNetLoadOnClient = true;

	// 写法2 演示（注释）：跳过 Client 端构造 —— 该 Actor 仅在 Server 存在
	// bNetLoadOnClient = false;
}

void AT24_NetLoadOnClientActor::BeginPlay()
{
	Super::BeginPlay();

	// 若蓝图勾了 Demo 开关，则在 BeginPlay 改一下，方便体感对比
	// 注意：bNetLoadOnClient 在构造期决定 Client 是否构造，运行时改动只对"未来情况"有效
	if (T24_bDemoNetLoadFalse)
	{
		bNetLoadOnClient = false;
	}

	NET_LOG(TEXT("BeginPlay bNetLoadOnClient=%d bReplicates=%d"),
		bNetLoadOnClient ? 1 : 0,
		GetIsReplicated() ? 1 : 0);
}
