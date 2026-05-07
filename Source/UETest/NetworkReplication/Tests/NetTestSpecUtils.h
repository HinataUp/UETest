// 网络同步示例 自动化测试 - 公共辅助
//
// 设计要点：
//   1. 所有测试代码包在 #if WITH_DEV_AUTOMATION_TESTS 宏内，发布版本不会编译进来
//   2. 用 Spec 风格（BDD-like），Describe / It 输出在 Session Frontend 中可读性最好
//   3. Describe 字符串用中文，便于直观查看
//
// 运行入口：
//   方式A：Editor → Window → Developer Tools → Session Frontend → Automation
//          搜索 "UETest.Network"，勾选并运行
//   方式B：命令行 UnrealEditor-Cmd.exe <uproject> -ExecCmds="Automation RunTests UETest.Network"

#pragma once

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"
#include "UObject/UnrealType.h"
#include "GameFramework/Actor.h"

namespace NetTestSpecUtils
{
	// 在 GetLifetimeReplicatedProps 输出列表里找指定字段
	// 用途：验证一个 UPROPERTY(Replicated) 字段确实被注册到了复制系统
	inline bool ContainsReplicatedProp(
		const TArray<FLifetimeProperty>& Props,
		const UClass* Class,
		const FName& FieldName)
	{
		// 反射拿到字段的 RepIndex
		FProperty* Prop = Class->FindPropertyByName(FieldName);
		if (!Prop) { return false; }
		const uint16 RepIndex = Prop->RepIndex;
		for (const FLifetimeProperty& L : Props)
		{
			if (L.RepIndex == RepIndex) { return true; }
		}
		return false;
	}

	// 反射检查一个函数是否带某些 FUNC_xxx 标记
	// 用途：验证 UFUNCTION(Server, Reliable, ...) 等标记真的生效
	inline bool FunctionHasFlags(
		const UClass* Class,
		const FName& FuncName,
		EFunctionFlags Required)
	{
		UFunction* Fn = Class->FindFunctionByName(FuncName);
		if (!Fn) { return false; }
		return EnumHasAllFlags(Fn->FunctionFlags, Required);
	}

	// 创建一个临时 Actor 实例做配置检查
	// 用 NewObject<T>() 创建 CDO 之外的实例（CDO 也行，但有的成员要在 Construct 后才稳定）
	template <typename T>
	T* MakeTempActor()
	{
		return NewObject<T>();
	}
}

#endif // WITH_DEV_AUTOMATION_TESTS
