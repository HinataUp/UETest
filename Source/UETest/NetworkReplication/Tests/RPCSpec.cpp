// RPC (T16-T20) 自动化测试
//
// 单测能验证：
//   * UFUNCTION(Server/Client/NetMulticast, Reliable/Unreliable, WithValidation) 标记是否正确生效
//   * _Implementation 业务逻辑 ：可直接调用，与 RPC 通道无关
//   * _Validate 范围检查 ：纯函数，最适合单测

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "NetTestSpecUtils.h"
#include "../T16_T20_RPC/T16_ServerRPCPawn.h"
#include "../T16_T20_RPC/T17_ClientRPCPawn.h"
#include "../T16_T20_RPC/T18_MulticastRPCActor.h"
#include "../T16_T20_RPC/T20_RPCValidationPawn.h"

BEGIN_DEFINE_SPEC(
	FUETest_RPC_Spec,
	"UETest.Network.RPC",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_RPC_Spec)

void FUETest_RPC_Spec::Define()
{
	using namespace NetTestSpecUtils;

	Describe(TEXT("T16 Server RPC"), [this]()
	{
		It(TEXT("ServerSendReliable 必须带 FUNC_NetServer + FUNC_NetReliable"), [this]()
		{
			TestTrue(TEXT("Reliable Server RPC 标记齐全"),
				FunctionHasFlags(AT16_ServerRPCPawn::StaticClass(),
					TEXT("ServerSendReliable"),
					FUNC_Net | FUNC_NetServer | FUNC_NetReliable));
		});

		It(TEXT("ServerSendUnreliable 不应带 FUNC_NetReliable"), [this]()
		{
			UFunction* Fn = AT16_ServerRPCPawn::StaticClass()->FindFunctionByName(
				TEXT("ServerSendUnreliable"));
			TestNotNull(TEXT("反射可达"), Fn);
			if (Fn)
			{
				TestTrue(TEXT("是 NetServer"),
					EnumHasAllFlags(Fn->FunctionFlags, FUNC_Net | FUNC_NetServer));
				TestFalse(TEXT("不是 Reliable"),
					EnumHasAllFlags(Fn->FunctionFlags, FUNC_NetReliable));
			}
		});
	});

	Describe(TEXT("T17 Client RPC"), [this]()
	{
		It(TEXT("ClientShowMessage 必须带 FUNC_NetClient"), [this]()
		{
			TestTrue(TEXT("Client RPC 标记正确"),
				FunctionHasFlags(AT17_ClientRPCPawn::StaticClass(),
					TEXT("ClientShowMessage"),
					FUNC_Net | FUNC_NetClient));
		});
	});

	Describe(TEXT("T18 Multicast RPC"), [this]()
	{
		It(TEXT("MulticastPlayVFX 必须带 FUNC_NetMulticast"), [this]()
		{
			TestTrue(TEXT("Multicast RPC 标记正确"),
				FunctionHasFlags(AT18_MulticastRPCActor::StaticClass(),
					TEXT("MulticastPlayVFX"),
					FUNC_Net | FUNC_NetMulticast));
		});
	});

	Describe(TEXT("T20 _Validate 校验逻辑"), [this]()
	{
		It(TEXT("合法参数 (10) 应通过校验"), [this]()
		{
			AT20_RPCValidationPawn* P = MakeTempActor<AT20_RPCValidationPawn>();
			TestTrue(TEXT("Damage=10 合法"),
				P->ServerSafeAction_Validate(10));
		});

		It(TEXT("过大参数 (99999) 应被校验拒绝"), [this]()
		{
			AT20_RPCValidationPawn* P = MakeTempActor<AT20_RPCValidationPawn>();
			TestFalse(TEXT("Damage=99999 越界"),
				P->ServerSafeAction_Validate(99999));
		});

		It(TEXT("负数应被拒绝"), [this]()
		{
			AT20_RPCValidationPawn* P = MakeTempActor<AT20_RPCValidationPawn>();
			TestFalse(TEXT("Damage=-1 非法"),
				P->ServerSafeAction_Validate(-1));
		});

		It(TEXT("UFUNCTION 必须带 FUNC_NetValidate"), [this]()
		{
			TestTrue(TEXT("WithValidation 应展开为 FUNC_NetValidate"),
				FunctionHasFlags(AT20_RPCValidationPawn::StaticClass(),
					TEXT("ServerSafeAction"),
					FUNC_NetValidate));
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
