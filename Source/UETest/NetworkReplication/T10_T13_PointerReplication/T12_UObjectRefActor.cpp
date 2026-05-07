// T12 - UObject 指针跨端失败示例 实现

#include "T12_UObjectRefActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT12_UObjectRefActor::AT12_UObjectRefActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT12_UObjectRefActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT12_UObjectRefActor, T12_InfoRef);
}

void AT12_UObjectRefActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T12_TickTimer, this, &AT12_UObjectRefActor::T12_ServerCreateAndAssign,
			2.0f, false);
	}

	// 三端 5 秒后再次记录状态，对比 Server / Client 看到的差异
	FTimerHandle DelayHandle;
	GetWorldTimerManager().SetTimer(
		DelayHandle,
		FTimerDelegate::CreateUObject(this, &AT12_UObjectRefActor::T12_LogState),
		5.0f, false);
}

void AT12_UObjectRefActor::T12_ServerCreateAndAssign()
{
	// NewObject 在 Server 端创建一个普通 UObject 实例
	// Outer 设为 this ，确保生命周期跟随 Actor
	T12_InfoRef = NewObject<UT12_PlainInfoObject>(this);
	if (T12_InfoRef)
	{
		T12_InfoRef->SomeValue = 42;
	}
	NET_LOG(TEXT("ServerCreated PlainObject Value=%d"),
		T12_InfoRef ? T12_InfoRef->SomeValue : -1);
}

void AT12_UObjectRefActor::T12_LogState()
{
	// 预期：Server 端非空且 Value=42 ；Client 端为 nullptr
	NET_LOG(TEXT("LogState InfoRef=%s"),
		T12_InfoRef ? *T12_InfoRef->GetName() : TEXT("nullptr <- 跨端失败示例"));
}
