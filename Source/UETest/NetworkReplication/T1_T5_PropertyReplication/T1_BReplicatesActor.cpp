// T1 - bReplicates 开关 与 Replicated 字段标记 实现

#include "T1_BReplicatesActor.h"
#include "Net/UnrealNetwork.h"   // DOREPLIFETIME 必需头文件
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT1_BReplicatesActor::AT1_BReplicatesActor()
	: T1_TestValue(0)
	, T1_NotReplicatedValue(0)
{
	PrimaryActorTick.bCanEverTick = false;

	// 写法A（推荐）：构造期开启复制
	bReplicates = true;

	// 写法B（备用，演示）：运行时切换
	// SetReplicates(true);
}

void AT1_BReplicatesActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME 宏展开后，会向复制系统注册该字段
	// 必须与 UPROPERTY(Replicated) 配套使用
	DOREPLIFETIME(AT1_BReplicatesActor, T1_TestValue);
	// 注意：T1_NotReplicatedValue 没有 Replicated 标记，这里不能写 DOREPLIFETIME，
	// 否则编译期 / 运行期会报 missing UPROPERTY
}

void AT1_BReplicatesActor::BeginPlay()
{
	Super::BeginPlay();

	// 仅在权威端启动定时器；Client 端不应自行修改权威数据
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T1_TickTimer,
			this, &AT1_BReplicatesActor::T1_ServerTickValue,
			1.0f, true);
	}

	NET_LOG(TEXT("BeginPlay TestValue=%d NotRep=%d"),
		T1_TestValue, T1_NotReplicatedValue);
}

void AT1_BReplicatesActor::T1_ServerTickValue()
{
	// 仅 Server 执行
	++T1_TestValue;
	++T1_NotReplicatedValue;

	// 三端都打印（Client 端 Tick 不会调用此函数，所以 Client 的值是从复制来的）
	NET_LOG(TEXT("ServerTick TestValue=%d NotRep=%d"),
		T1_TestValue, T1_NotReplicatedValue);
}
