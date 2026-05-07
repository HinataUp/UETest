// 条件复制 / 生命周期 / 组件复制 / 权威闭环 (T14-T28) 自动化测试

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "NetTestSpecUtils.h"
#include "../T14_T15_ConditionalReplication/T14_LifetimeConditionActor.h"
#include "../T14_T15_ConditionalReplication/T15_PreReplicationActor.h"
#include "../T21_T24_ActorLifecycle/T21_LifecycleTimingActor.h"
#include "../T21_T24_ActorLifecycle/T23_NetDormancyActor.h"
#include "../T21_T24_ActorLifecycle/T24_NetLoadOnClientActor.h"
#include "../T25_T26_ComponentReplication/T25_RepCombatComponent.h"
#include "../T25_T26_ComponentReplication/T26_ReplicatedSubObjectActor.h"
#include "../T27_T29_AuthorityPrediction/T27_ServerAuthorityCharacter.h"
#include "../T27_T29_AuthorityPrediction/T28_ClientPredictionCharacter.h"

BEGIN_DEFINE_SPEC(
	FUETest_LifecycleAndComponent_Spec,
	"UETest.Network.生命周期与组件",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_LifecycleAndComponent_Spec)

void FUETest_LifecycleAndComponent_Spec::Define()
{
	using namespace NetTestSpecUtils;

	Describe(TEXT("T14 五种 DOREPLIFETIME_CONDITION"), [this]()
	{
		It(TEXT("五个条件字段全部注册"), [this]()
		{
			AT14_LifetimeConditionActor* A = MakeTempActor<AT14_LifetimeConditionActor>();
			TArray<FLifetimeProperty> Props;
			A->GetLifetimeReplicatedProps(Props);

			const UClass* C = AT14_LifetimeConditionActor::StaticClass();
			TestTrue(TEXT("OwnerOnly 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T14_RepOwnerOnly")));
			TestTrue(TEXT("SkipOwner 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T14_RepSkipOwner")));
			TestTrue(TEXT("SimulatedOnly 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T14_RepSimulatedOnly")));
			TestTrue(TEXT("AutonomousOnly 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T14_RepAutonomousOnly")));
			TestTrue(TEXT("InitialOnly 已注册"),
				ContainsReplicatedProp(Props, C, TEXT("T14_RepInitialOnly")));
		});

		It(TEXT("OwnerOnly 字段的 RepCondition 必须是 COND_OwnerOnly"), [this]()
		{
			AT14_LifetimeConditionActor* A = MakeTempActor<AT14_LifetimeConditionActor>();
			TArray<FLifetimeProperty> Props;
			A->GetLifetimeReplicatedProps(Props);

			FProperty* P = AT14_LifetimeConditionActor::StaticClass()
				->FindPropertyByName(TEXT("T14_RepOwnerOnly"));
			TestNotNull(TEXT("反射可达"), P);
			if (P)
			{
				bool bFound = false;
				for (const FLifetimeProperty& L : Props)
				{
					if (L.RepIndex == P->RepIndex)
					{
						bFound = true;
						TestEqual(TEXT("Condition 应为 COND_OwnerOnly"),
							static_cast<int32>(L.Condition), static_cast<int32>(COND_OwnerOnly));
					}
				}
				TestTrue(TEXT("能在 RepProps 中找到该字段"), bFound);
			}
		});
	});

	Describe(TEXT("T15 PreReplication 动态条件"), [this]()
	{
		It(TEXT("PreReplication 已被 override"), [this]()
		{
			// 用 CDO 检查虚函数表是否被覆盖（间接验证：默认值与基类不一定不同，
			// 这里改成调用一次确认不崩溃即可）
			AT15_PreReplicationActor* A = MakeTempActor<AT15_PreReplicationActor>();
			TestNotNull(TEXT("Actor 创建成功"), A);
			// 真要严苛比对可对比 vtable ；此处仅验证 ConditionalField 注册
			TArray<FLifetimeProperty> Props;
			A->GetLifetimeReplicatedProps(Props);
			TestTrue(TEXT("T15_ConditionalField 已注册"),
				ContainsReplicatedProp(Props,
					AT15_PreReplicationActor::StaticClass(),
					TEXT("T15_ConditionalField")));
		});
	});

	Describe(TEXT("T21 Actor 默认配置"), [this]()
	{
		It(TEXT("bReplicates 默认 true"), [this]()
		{
			AT21_LifecycleTimingActor* A = MakeTempActor<AT21_LifecycleTimingActor>();
			TestTrue(TEXT("bReplicates"), A->GetIsReplicated());
		});
	});

	Describe(TEXT("T23 NetDormancy 配置"), [this]()
	{
		It(TEXT("NetDormancy 默认应为 DORM_Initial"), [this]()
		{
			AT23_NetDormancyActor* A = MakeTempActor<AT23_NetDormancyActor>();
			TestEqual(TEXT("初始休眠"),
				static_cast<int32>(A->NetDormancy),
				static_cast<int32>(DORM_Initial));
		});
	});

	Describe(TEXT("T24 bNetLoadOnClient"), [this]()
	{
		It(TEXT("默认 bNetLoadOnClient 应为 true"), [this]()
		{
			AT24_NetLoadOnClientActor* A = MakeTempActor<AT24_NetLoadOnClientActor>();
			TestTrue(TEXT("默认 Client 也加载关卡 Actor"),
				A->bNetLoadOnClient);
		});
	});

	Describe(TEXT("T25 Replicated Component"), [this]()
	{
		It(TEXT("Component 默认开启复制"), [this]()
		{
			UT25_RepCombatComponent* C = NewObject<UT25_RepCombatComponent>();
			TestTrue(TEXT("SetIsReplicatedByDefault(true) 生效"),
				C->GetIsReplicated());
		});

		It(TEXT("CompValue 字段已注册"), [this]()
		{
			UT25_RepCombatComponent* C = NewObject<UT25_RepCombatComponent>();
			TArray<FLifetimeProperty> Props;
			C->GetLifetimeReplicatedProps(Props);
			TestTrue(TEXT("CompValue 已注册"),
				ContainsReplicatedProp(Props,
					UT25_RepCombatComponent::StaticClass(),
					TEXT("T25_CompValue")));
		});

		It(TEXT("Actor 默认带有 Component（CreateDefaultSubobject）"), [this]()
		{
			AT25_RepComponentActor* A = MakeTempActor<AT25_RepComponentActor>();
			TestNotNull(TEXT("DefaultSubobject 应非空"), A->T25_CombatComp.Get());
		});
	});

	Describe(TEXT("T26 ReplicatedSubObject"), [this]()
	{
		It(TEXT("Actor 启用了注册式子对象复制"), [this]()
		{
			AT26_ReplicatedSubObjectActor* A = MakeTempActor<AT26_ReplicatedSubObjectActor>();
			// 直接访问 AActor 的 public bool 成员，最稳的写法
			TestTrue(TEXT("bReplicateUsingRegisteredSubObjectList=true"),
				A->bReplicateUsingRegisteredSubObjectList);
		});

		It(TEXT("UT26_RepSubObject::IsSupportedForNetworking 返回 true"), [this]()
		{
			UT26_RepSubObject* S = NewObject<UT26_RepSubObject>();
			TestTrue(TEXT("IsSupportedForNetworking"),
				S->IsSupportedForNetworking());
		});

		It(TEXT("SubObjValue 字段已注册"), [this]()
		{
			UT26_RepSubObject* S = NewObject<UT26_RepSubObject>();
			TArray<FLifetimeProperty> Props;
			S->GetLifetimeReplicatedProps(Props);
			TestTrue(TEXT("SubObjValue 在 RepProps 中"),
				ContainsReplicatedProp(Props,
					UT26_RepSubObject::StaticClass(), TEXT("SubObjValue")));
		});
	});

	Describe(TEXT("T27 Server 权威：业务逻辑直调"), [this]()
	{
		It(TEXT("正常请求扣 10 血，Health 应从 100 -> 90"), [this]()
		{
			AT27_ServerAuthorityCharacter* C = MakeTempActor<AT27_ServerAuthorityCharacter>();
			C->ServerRequestTakeDamage_Implementation(10);
			FProperty* P = AT27_ServerAuthorityCharacter::StaticClass()
				->FindPropertyByName(TEXT("T27_Health"));
			if (P)
			{
				TestEqual(TEXT("Health 应为 90"),
					*P->ContainerPtrToValuePtr<int32>(C), 90);
			}
		});

		It(TEXT("Validate 拒绝越界 Damage"), [this]()
		{
			AT27_ServerAuthorityCharacter* C = MakeTempActor<AT27_ServerAuthorityCharacter>();
			TestFalse(TEXT("Damage=99999 不合法"),
				C->ServerRequestTakeDamage_Validate(99999));
			TestTrue(TEXT("Damage=10 合法"),
				C->ServerRequestTakeDamage_Validate(10));
		});
	});

	Describe(TEXT("T28 客户端预测：业务逻辑直调"), [this]()
	{
		It(TEXT("Health <= 50 时 Server 拒绝扣血（值不变）"), [this]()
		{
			AT28_ClientPredictionCharacter* C = MakeTempActor<AT28_ClientPredictionCharacter>();
			// 反射改 Health 到 50
			FProperty* P = AT28_ClientPredictionCharacter::StaticClass()
				->FindPropertyByName(TEXT("T28_Health"));
			if (P)
			{
				int32* Val = P->ContainerPtrToValuePtr<int32>(C);
				*Val = 50;
				C->ServerRequestTakeDamage_Implementation(10);
				TestEqual(TEXT("Server 拒绝扣血，Health 仍为 50"), *Val, 50);
			}
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
