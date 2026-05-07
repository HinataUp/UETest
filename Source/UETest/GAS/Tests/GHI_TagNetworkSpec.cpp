// G / H / I 章节 自动化测试 ：Tag 系统、集成、网络

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "GASTestSpecUtils.h"
#include "../G_TagSystem/G36_LooseVsGrantedTag.h"
#include "../H_Integration/H38_GE_BerserkerBuff.h"
#include "../I_Network/I46_GA_PredictionRollback.h"
#include "../00_Common/MyGameplayTags.h"

BEGIN_DEFINE_SPEC(
	FUETest_GAS_GHI_Spec,
	"UETest.GAS.G_Tag与H_集成与I_网络",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_GAS_GHI_Spec)

void FUETest_GAS_GHI_Spec::Define()
{
	using namespace GASTestSpecUtils;

	Describe(TEXT("G36 Loose vs Granted Tag"), [this]()
	{
		It(TEXT("AddLooseGameplayTag 本地端可读"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.ASC->AddLooseGameplayTag(MyGameplayTags::TAG_State_Test);
			TestTrue(TEXT("【Loose】本地端有 Tag"),
				Env.ASC->HasMatchingGameplayTag(MyGameplayTags::TAG_State_Test));

			Env.ASC->RemoveLooseGameplayTag(MyGameplayTags::TAG_State_Test);
			TestFalse(TEXT("【Loose】Remove 后没有 Tag"),
				Env.ASC->HasMatchingGameplayTag(MyGameplayTags::TAG_State_Test));
		});

		It(TEXT("GE-Driven Tag 随 GE 生命周期"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			auto H = ApplyGEToSelf(Env.ASC, UGE_GrantTestTag::StaticClass());
			TestTrue(TEXT("【GE-Tag】Apply 后有 Tag"),
				Env.ASC->HasMatchingGameplayTag(MyGameplayTags::TAG_State_Test));
			Env.ASC->RemoveActiveGameplayEffect(H);
			TestFalse(TEXT("【GE-Tag】GE 移除后 Tag 消失"),
				Env.ASC->HasMatchingGameplayTag(MyGameplayTags::TAG_State_Test));
		});
	});

	Describe(TEXT("H38 GE 授予 / 收回 GA"), [this]()
	{
		It(TEXT("BerserkerBuff Apply 后 ASC 含 GA"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			TestNull(TEXT("【授技能】Apply 前 ASC 无 BerserkerSlash"),
				Env.ASC->FindAbilitySpecFromClass(UGA_BerserkerSlash::StaticClass()));

			ApplyGEToSelf(Env.ASC, UGE_BerserkerBuff::StaticClass());

			TestNotNull(TEXT("【授技能】Apply 后 ASC 应含 BerserkerSlash"),
				Env.ASC->FindAbilitySpecFromClass(UGA_BerserkerSlash::StaticClass()));
		});
	});

	Describe(TEXT("I46 PredictionRollback GA"), [this]()
	{
		It(TEXT("LocalPredicted 配置正确"), [this]()
		{
			UGA_PredictionRollback* GA = NewObject<UGA_PredictionRollback>();
			TestEqual(TEXT("【预测】NetExecutionPolicy=LocalPredicted"),
				static_cast<int32>(GA->GetNetExecutionPolicy()),
				static_cast<int32>(EGameplayAbilityNetExecutionPolicy::LocalPredicted));
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
