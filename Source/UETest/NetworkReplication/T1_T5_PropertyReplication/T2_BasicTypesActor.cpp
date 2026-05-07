// T2 - 基础数据类型复制 实现

#include "T2_BasicTypesActor.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "../Common/NetTestLogMacros.h"

AT2_BasicTypesActor::AT2_BasicTypesActor()
	: T2_RepInt(0)
	, T2_RepFloat(0.f)
	, T2_bRepBool(false)
	, T2_RepString(TEXT("Init"))
	, T2_RepName(TEXT("None"))
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AT2_BasicTypesActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 写法1：最常用的标准复制声明
	DOREPLIFETIME(AT2_BasicTypesActor, T2_RepInt);
	DOREPLIFETIME(AT2_BasicTypesActor, T2_RepFloat);
	DOREPLIFETIME(AT2_BasicTypesActor, T2_bRepBool);
	DOREPLIFETIME(AT2_BasicTypesActor, T2_RepString);
	DOREPLIFETIME(AT2_BasicTypesActor, T2_RepName);

	// 写法2 演示（注释）：UE5 推荐的高性能版本
	// FDoRepLifetimeParams Params;
	// Params.bIsPushBased = true;            // Push Model：值未脏不参与扫描
	// DOREPLIFETIME_WITH_PARAMS_FAST(AT2_BasicTypesActor, T2_RepInt, Params);
}

void AT2_BasicTypesActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			T2_TickTimer, this, &AT2_BasicTypesActor::T2_ServerTickAll,
			2.0f, true);
	}
}

void AT2_BasicTypesActor::T2_ServerTickAll()
{
	// Server 端统一更新；Client 端通过复制收到
	++T2_RepInt;
	T2_RepFloat += 0.5f;
	T2_bRepBool = !T2_bRepBool;
	T2_RepString = FString::Printf(TEXT("Tick_%d"), T2_RepInt);
	// FName 复制成本极低，因为底层是全局表索引，不是字符串本身
	T2_RepName = FName(*FString::Printf(TEXT("State_%d"), T2_RepInt));

	NET_LOG(TEXT("Tick i=%d f=%.2f b=%d s=%s n=%s"),
		T2_RepInt, T2_RepFloat, T2_bRepBool, *T2_RepString, *T2_RepName.ToString());
}
