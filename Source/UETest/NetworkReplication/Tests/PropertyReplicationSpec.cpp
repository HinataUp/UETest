// 属性复制 (T1-T5) 自动化测试

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "NetTestSpecUtils.h"
#include "../T1_T5_PropertyReplication/T1_BReplicatesActor.h"
#include "../T1_T5_PropertyReplication/T2_BasicTypesActor.h"
#include "../T1_T5_PropertyReplication/T3_StructActor.h"
#include "../T1_T5_PropertyReplication/T4_ArrayActor.h"
#include "../T1_T5_PropertyReplication/T5_EnumNameActor.h"

// Spec 命名规范：FUETest_<分类>_Spec
// 路径 "UETest.Network.属性复制" 在 SessionFrontend 中即对应树形目录
BEGIN_DEFINE_SPEC(
	FUETest_PropertyReplication_Spec,
	"UETest.Network.属性复制",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_PropertyReplication_Spec)

void FUETest_PropertyReplication_Spec::Define()
{
	using namespace NetTestSpecUtils;

	Describe(TEXT("T1 bReplicates 与 Replicated 标记"), [this]()
	{
		It(TEXT("Actor 默认应启用 bReplicates"), [this]()
		{
			AT1_BReplicatesActor* A = MakeTempActor<AT1_BReplicatesActor>();
			TestTrue(TEXT("bReplicates 必须为 true"), A->GetIsReplicated());
		});

		It(TEXT("T1_TestValue 字段必须被注册到复制系统"), [this]()
		{
			AT1_BReplicatesActor* A = MakeTempActor<AT1_BReplicatesActor>();
			TArray<FLifetimeProperty> Props;
			A->GetLifetimeReplicatedProps(Props);
			TestTrue(
				TEXT("DOREPLIFETIME(T1_TestValue) 必须出现在 GetLifetimeReplicatedProps 中"),
				ContainsReplicatedProp(Props, AT1_BReplicatesActor::StaticClass(),
					TEXT("T1_TestValue")));
		});

		It(TEXT("没标 Replicated 的字段不应被注册"), [this]()
		{
			AT1_BReplicatesActor* A = MakeTempActor<AT1_BReplicatesActor>();
			TArray<FLifetimeProperty> Props;
			A->GetLifetimeReplicatedProps(Props);
			TestFalse(
				TEXT("T1_NotReplicatedValue 不应在复制列表中"),
				ContainsReplicatedProp(Props, AT1_BReplicatesActor::StaticClass(),
					TEXT("T1_NotReplicatedValue")));
		});
	});

	Describe(TEXT("T2 基础数据类型 (int / float / bool / FString / FName)"), [this]()
	{
		It(TEXT("五个字段必须全部注册到复制系统"), [this]()
		{
			AT2_BasicTypesActor* A = MakeTempActor<AT2_BasicTypesActor>();
			TArray<FLifetimeProperty> Props;
			A->GetLifetimeReplicatedProps(Props);

			const UClass* C = AT2_BasicTypesActor::StaticClass();
			TestTrue(TEXT("T2_RepInt 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T2_RepInt")));
			TestTrue(TEXT("T2_RepFloat 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T2_RepFloat")));
			TestTrue(TEXT("T2_bRepBool 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T2_bRepBool")));
			TestTrue(TEXT("T2_RepString 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T2_RepString")));
			TestTrue(TEXT("T2_RepName 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T2_RepName")));
		});
	});

	Describe(TEXT("T3 USTRUCT 复制"), [this]()
	{
		It(TEXT("T3_CombatState 字段必须注册到复制系统"), [this]()
		{
			AT3_StructActor* A = MakeTempActor<AT3_StructActor>();
			TArray<FLifetimeProperty> Props;
			A->GetLifetimeReplicatedProps(Props);
			TestTrue(TEXT("整个结构体作为单元注册"),
				ContainsReplicatedProp(Props, AT3_StructActor::StaticClass(),
					TEXT("T3_CombatState")));
		});

		It(TEXT("FT3_CombatState 字段反射可读"), [this]()
		{
			UScriptStruct* S = FT3_CombatState::StaticStruct();
			TestNotNull(TEXT("StaticStruct 必须有效"), S);
			TestNotNull(TEXT("Health 字段反射可达"),
				S->FindPropertyByName(TEXT("Health")));
			TestNotNull(TEXT("Stamina 字段反射可达"),
				S->FindPropertyByName(TEXT("Stamina")));
			TestNotNull(TEXT("bBlocking 字段反射可达"),
				S->FindPropertyByName(TEXT("bBlocking")));
		});
	});

	Describe(TEXT("T4 TArray 容器复制"), [this]()
	{
		It(TEXT("两个 TArray 字段必须注册到复制系统"), [this]()
		{
			AT4_ArrayActor* A = MakeTempActor<AT4_ArrayActor>();
			TArray<FLifetimeProperty> Props;
			A->GetLifetimeReplicatedProps(Props);

			const UClass* C = AT4_ArrayActor::StaticClass();
			TestTrue(TEXT("T4_RepIntArray 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T4_RepIntArray")));
			TestTrue(TEXT("T4_RepStateArray 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T4_RepStateArray")));
		});
	});

	Describe(TEXT("T5 枚举 / FName 复制"), [this]()
	{
		It(TEXT("ET5_CombatPhase 必须是 uint8 底层（UE 反射强制）"), [this]()
		{
			UEnum* E = StaticEnum<ET5_CombatPhase>();
			TestNotNull(TEXT("StaticEnum 必须有效"), E);
			// uint8 底层 -> 单元素长度 = 1
			TestEqual(TEXT("ET5_CombatPhase 必须为 uint8"),
				static_cast<int32>(sizeof(ET5_CombatPhase)), 1);
		});

		It(TEXT("Phase 与 StateName 字段已注册"), [this]()
		{
			AT5_EnumNameActor* A = MakeTempActor<AT5_EnumNameActor>();
			TArray<FLifetimeProperty> Props;
			A->GetLifetimeReplicatedProps(Props);

			const UClass* C = AT5_EnumNameActor::StaticClass();
			TestTrue(TEXT("T5_Phase 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T5_Phase")));
			TestTrue(TEXT("T5_StateName 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T5_StateName")));
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
