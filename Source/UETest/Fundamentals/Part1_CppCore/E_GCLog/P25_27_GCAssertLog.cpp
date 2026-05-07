#include "P25_27_GCAssertLog.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogP1_GCAssert);

void UP25_27_GCAssertLog::ForceFullGC()
{
	if (GEngine)
	{
		// 强制一次完整 GC（同步 ）；测试时可用 ，正式项目慎用 ：会让游戏卡顿
		GEngine->ForceGarbageCollection(/*bForceFullPurge=*/true);
	}
}

void UP25_27_GCAssertLog::AddToRoot(UObject* Obj)
{
	if (Obj && !Obj->IsRooted())
	{
		Obj->AddToRoot();
	}
}

void UP25_27_GCAssertLog::RemoveFromRoot(UObject* Obj)
{
	if (Obj && Obj->IsRooted())
	{
		Obj->RemoveFromRoot();
	}
}

void UP25_27_GCAssertLog::DemoEnsure(int32 Value)
{
	// ensure ：失败打 callstack 但不崩溃 ；适合"严重但可继续"的异常
	// check ：失败崩溃 ；适合"绝不可能"的非法状态（构造期 / 不变量）
	// verify ：Shipping 也执行表达式（保留副作用）
	ensureMsgf(Value > 0, TEXT("【P26】期望 Value > 0，实际 %d"), Value);
}

void UP25_27_GCAssertLog::DemoLog(const FString& Message)
{
	// 五级日志 ：Verbose / Log / Display / Warning / Error
	// 控制台 "log LogP1_GCAssert Verbose" 打开全部
	UE_LOG(LogP1_GCAssert, Log, TEXT("【P27】Log 级 ：%s"), *Message);
	UE_LOG(LogP1_GCAssert, Warning, TEXT("【P27】Warning 级"));
	UE_LOG(LogP1_GCAssert, Error, TEXT("【P27】Error 级"));
}
