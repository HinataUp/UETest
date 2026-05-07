// 网络同步示例公共日志宏
// 用途：在三端（Server/Client）打印统一格式的日志，方便观察 Role / NetMode
// 使用：在任意 Actor 内 #include 本文件，然后 NET_LOG(TEXT("Hello %d"), 42)

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

// 取得当前 NetMode 的可读字符串（Listen / Client / DedicatedServer / Standalone）
// 该函数为 inline，避免重复链接错误；放在头文件里便于被各 task 共享
inline FString GetNetModeStr_NetTest(const UObject* WorldContextObject)
{
	if (!WorldContextObject) { return TEXT("NoWorld"); }
	const UWorld* MyWorld = WorldContextObject->GetWorld();
	if (!MyWorld) { return TEXT("NoWorld"); }
	switch (MyWorld->GetNetMode())
	{
	case NM_DedicatedServer: return TEXT("DediSrv");
	case NM_ListenServer:    return TEXT("Listen");
	case NM_Client:          return TEXT("Client");
	case NM_Standalone:      return TEXT("Standalone");
	default:                 return TEXT("Unknown");
	}
}

// 取得 Actor 的 LocalRole 字符串（ROLE_Authority / ROLE_AutonomousProxy / ROLE_SimulatedProxy / ROLE_None）
inline FString GetRoleStr_NetTest(const AActor* InActor)
{
	if (!InActor) { return TEXT("NoActor"); }
	// 注意：UEnum::GetValueAsString 对反射枚举有效，ENetRole 是 UENUM
	return UEnum::GetValueAsString(InActor->GetLocalRole());
}

// 主日志宏：自动带上 [NetMode|Role|ActorName] 前缀
// 设计取舍：用 Warning 等级方便在 Output Log 里以颜色区分；正式工程应换成自定义 LogCategory
#define NET_LOG(Format, ...) \
	UE_LOG(LogTemp, Warning, TEXT("[%s|%s|%s] " Format), \
		*GetNetModeStr_NetTest(this), \
		*GetRoleStr_NetTest(this), \
		*GetName(), \
		##__VA_ARGS__)

// 屏幕日志宏（开发期可视化用，正式工程禁用）
#define NET_SCREEN_LOG(Color, Format, ...) \
	if (GEngine) \
	{ \
		GEngine->AddOnScreenDebugMessage(-1, 5.f, Color, \
			FString::Printf(TEXT("[%s|%s] " Format), \
				*GetNetModeStr_NetTest(this), \
				*GetRoleStr_NetTest(this), \
				##__VA_ARGS__)); \
	}
