// RepNotify (T6-T9) 自动化测试
//
// 单测能验证什么：
//   * OnRep 函数有 UFUNCTION 标记，能被反射找到
//   * OnRep 签名正确（无参 / 带 OldValue）
//   * 业务逻辑可直接调用（OnRep 函数本身就是一个普通成员函数）
// 单测不能验证什么：
//   * OnRep 在远端是否真的被复制层自动调用 → 留给手动 PIE

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "NetTestSpecUtils.h"
#include "../T6_T9_RepNotify/T6_OnRepBasicActor.h"
#include "../T6_T9_RepNotify/T7_OnRepWithOldValueActor.h"
#include "../T6_T9_RepNotify/T8_RepNotifyAlwaysActor.h"
#include "../T6_T9_RepNotify/T9_BatchedChangesActor.h"

BEGIN_DEFINE_SPEC(
	FUETest_RepNotify_Spec,
	"UETest.Network.RepNotify",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_RepNotify_Spec)

void FUETest_RepNotify_Spec::Define()
{
	using namespace NetTestSpecUtils;

	Describe(TEXT("T6 无参 OnRep"), [this]()
	{
		It(TEXT("OnRep_T6_RepInt 函数被反射注册"), [this]()
		{
			UFunction* Fn = AT6_OnRepBasicActor::StaticClass()->FindFunctionByName(
				TEXT("OnRep_T6_RepInt"));
			TestNotNull(TEXT("反射应能找到 OnRep 函数（必须加 UFUNCTION()）"), Fn);
		});

		It(TEXT("T6_RepInt 字段已注册到复制系统"), [this]()
		{
			AT6_OnRepBasicActor* A = MakeTempActor<AT6_OnRepBasicActor>();
			TArray<FLifetimeProperty> Props;
			A->GetLifetimeReplicatedProps(Props);
			TestTrue(TEXT("T6_RepInt 在 RepProps 列表中"),
				ContainsReplicatedProp(Props,
					AT6_OnRepBasicActor::StaticClass(), TEXT("T6_RepInt")));
		});
	});

	Describe(TEXT("T7 带 OldValue 的 OnRep"), [this]()
	{
		It(TEXT("OnRep_T7_Health 函数被反射注册且能取到一个参数"), [this]()
		{
			UFunction* Fn = AT7_OnRepWithOldValueActor::StaticClass()->FindFunctionByName(
				TEXT("OnRep_T7_Health"));
			TestNotNull(TEXT("反射可达"), Fn);
			if (Fn)
			{
				// 带 OldValue 的 OnRep 必须正好 1 个参数
				TestEqual(TEXT("参数数量应为 1（OldValue）"),
					static_cast<int32>(Fn->NumParms), 1);
			}
		});
	});

	Describe(TEXT("T8 REPNOTIFY_Always 标准模式"), [this]()
	{
		It(TEXT("Server 端手动调用 OnRep 不会崩溃，能正常更新内部状态"), [this]()
		{
			AT8_RepNotifyAlwaysActor* A = MakeTempActor<AT8_RepNotifyAlwaysActor>();
			// 直接调 Server 端业务函数：OldStacks=0, NewStacks=3
			A->T8_ServerRefreshAura(3);
			// Stacks 应被改成新值（业务逻辑层面的检查）
			// 反射读 T8_AuraStacks
			FProperty* P = AT8_RepNotifyAlwaysActor::StaticClass()->FindPropertyByName(
				TEXT("T8_AuraStacks"));
			TestNotNull(TEXT("T8_AuraStacks 反射可达"), P);
			if (P)
			{
				const int32* Val = P->ContainerPtrToValuePtr<int32>(A);
				TestEqual(TEXT("T8_AuraStacks 应被业务函数改写为 3"), *Val, 3);
			}
		});
	});

	Describe(TEXT("T9 同帧多次修改的合并行为"), [this]()
	{
		It(TEXT("HitHistory 数组字段已注册"), [this]()
		{
			AT9_BatchedChangesActor* A = MakeTempActor<AT9_BatchedChangesActor>();
			TArray<FLifetimeProperty> Props;
			A->GetLifetimeReplicatedProps(Props);
			TestTrue(TEXT("HitCount 已注册"),
				ContainsReplicatedProp(Props,
					AT9_BatchedChangesActor::StaticClass(), TEXT("T9_HitCount")));
			TestTrue(TEXT("HitHistory 已注册（用数组保留事件序列）"),
				ContainsReplicatedProp(Props,
					AT9_BatchedChangesActor::StaticClass(), TEXT("T9_HitHistory")));
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
