// T26 - ReplicatedSubObject 实现
// 演示新 API（UE5.1+ 推荐）：AddReplicatedSubObject
// 旧 API（仍可用）：override ReplicateSubobjects ，文末有注释示例

#include "T26_ReplicatedSubObjectActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

void UT26_RepSubObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UT26_RepSubObject, SubObjValue);
}

AT26_ReplicatedSubObjectActor::AT26_ReplicatedSubObjectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	// UE5 推荐：开启"注册式子对象复制"
	// 启用后必须使用 AddReplicatedSubObject ，不再走 ReplicateSubobjects
	bReplicateUsingRegisteredSubObjectList = true;
}

void AT26_ReplicatedSubObjectActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT26_ReplicatedSubObjectActor, T26_SubObj);
}

void AT26_ReplicatedSubObjectActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// 异步等一帧创建子对象，让 Actor 自己先完成基础复制握手
		FTimerHandle DelayHandle;
		GetWorldTimerManager().SetTimer(
			DelayHandle, this, &AT26_ReplicatedSubObjectActor::T26_ServerInitAndTick,
			0.5f, false);
	}
}

void AT26_ReplicatedSubObjectActor::T26_ServerInitAndTick()
{
	// Server 端创建 SubObject 并注册到复制系统
	T26_SubObj = NewObject<UT26_RepSubObject>(this);
	if (T26_SubObj)
	{
		// 关键：注册后，UE 会把它跟随本 Actor 一起复制到 Client
		AddReplicatedSubObject(T26_SubObj);
	}

	GetWorldTimerManager().SetTimer(
		T26_TickTimer, this, &AT26_ReplicatedSubObjectActor::T26_ServerTickValue,
		2.0f, true);
}

void AT26_ReplicatedSubObjectActor::T26_ServerTickValue()
{
	if (T26_SubObj)
	{
		++T26_SubObj->SubObjValue;
		NET_LOG(TEXT("ServerTick SubObj.Value=%d"), T26_SubObj->SubObjValue);
	}
}

// 旧 API 写法（仅作参考，与新 API 二选一）：
//
// bool AT26_ReplicatedSubObjectActor::ReplicateSubobjects(UActorChannel* Channel,
//     FOutBunch* Bunch, FReplicationFlags* RepFlags)
// {
//     bool bWrote = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
//     if (T26_SubObj)
//     {
//         bWrote |= Channel->ReplicateSubobject(T26_SubObj, *Bunch, *RepFlags);
//     }
//     return bWrote;
// }
//
// 同时构造期不要设置 bReplicateUsingRegisteredSubObjectList ，旧 API 与新 API 互斥
