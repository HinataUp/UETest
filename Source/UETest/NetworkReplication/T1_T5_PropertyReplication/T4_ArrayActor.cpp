// T4 - TArray 容器复制 实现

#include "T4_ArrayActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT4_ArrayActor::AT4_ArrayActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT4_ArrayActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AT4_ArrayActor, T4_RepIntArray);
	DOREPLIFETIME(AT4_ArrayActor, T4_RepStateArray);
}

void AT4_ArrayActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T4_TickTimer, this, &AT4_ArrayActor::T4_ServerTickArray,
			2.0f, true);
	}
}

void AT4_ArrayActor::T4_ServerTickArray()
{
	// 用 TickCount 模拟"增/改/删"四个阶段，循环演示
	switch (T4_TickCount % 4)
	{
	case 0:
		// 写法1：Add ：尾部追加
		T4_RepIntArray.Add(10 * (T4_TickCount + 1));
		T4_RepStateArray.Add(FT3_CombatState());
		break;

	case 1:
		// 写法2：原地修改下标元素
		if (T4_RepIntArray.IsValidIndex(0))
		{
			T4_RepIntArray[0] = 99;
		}
		if (T4_RepStateArray.IsValidIndex(0))
		{
			T4_RepStateArray[0].Health = 50;
		}
		break;

	case 2:
		// 写法3：RemoveAt ：按下标删除
		if (T4_RepIntArray.Num() > 0)
		{
			T4_RepIntArray.RemoveAt(0);
		}
		break;

	case 3:
		// 写法4：清空 —— 比逐个 RemoveAt 网络成本更小
		T4_RepIntArray.Empty();
		T4_RepStateArray.Empty();
		break;
	}

	++T4_TickCount;

	// 拼出可读字符串便于日志观察
	FString IntStr;
	for (int32 V : T4_RepIntArray) { IntStr += FString::Printf(TEXT("%d,"), V); }
	NET_LOG(TEXT("ArrayTick step=%d Ints=[%s] StatesNum=%d"),
		T4_TickCount, *IntStr, T4_RepStateArray.Num());
}
