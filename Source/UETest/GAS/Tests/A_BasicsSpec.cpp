// A 章节 自动化测试

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "GASTestSpecUtils.h"
#include "../A_Basics/A03_GE_InitPlayerStats.h"
#include "../00_Common/MyGameplayTags.h"

BEGIN_DEFINE_SPEC(
	FUETest_GAS_A_Basics_Spec,
	"UETest.GAS.A基础设施",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_GAS_A_Basics_Spec)

void FUETest_GAS_A_Basics_Spec::Define()
{
	using namespace GASTestSpecUtils;

	Describe(TEXT("A01 AttributeSet 基础"), [this]()
	{
		It(TEXT("无 Modifier 时 Base 应等于 Current"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			TestTrue(TEXT("环境构建成功"), Env.IsValid());
			if (!Env.IsValid()) return;

			Env.ASC->SetNumericAttributeBase(UHealthSet::GetHealthAttribute(), 100.f);
			const float Base = Env.ASC->GetNumericAttributeBase(UHealthSet::GetHealthAttribute());
			const float Current = Env.ASC->GetNumericAttribute(UHealthSet::GetHealthAttribute());
			TestEqual(TEXT("【属性基础】Base 应为 100"), Base, 100.f);
			TestEqual(TEXT("【属性基础】无 Modifier 时 Base==Current"), Base, Current);
		});
	});

	Describe(TEXT("A02 多 AttributeSet 共存"), [this]()
	{
		It(TEXT("HealthSet 与 CombatSet 都已注册"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			TestNotNull(TEXT("【多Set】HealthSet 应已注册"), Env.ASC->GetSet<UHealthSet>());
			TestNotNull(TEXT("【多Set】CombatSet 应已注册"), Env.ASC->GetSet<UCombatSet>());
		});
	});

	Describe(TEXT("A03 GE 初始化 vs Native InitXxx"), [this]()
	{
		It(TEXT("Native InitHealth 不触发 PostExecute"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.HealthSet->bDebugPostExecuteFired = false;
			Env.HealthSet->InitHealth(80.f);
			TestEqual(TEXT("【初始化-Native】Health Base=80"),
				Env.ASC->GetNumericAttributeBase(UHealthSet::GetHealthAttribute()), 80.f);
			TestFalse(TEXT("【初始化-Native】Pre/Post 不触发"),
				Env.HealthSet->bDebugPostExecuteFired);
		});

		It(TEXT("GE_InitPlayerStats 走完整管线，触发 PostExecute"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.HealthSet->bDebugPostExecuteFired = false;
			ApplyGEToSelf(Env.ASC, UGE_InitPlayerStats::StaticClass());
			TestEqual(TEXT("【初始化-GE】Health Base=100"),
				Env.ASC->GetNumericAttributeBase(UHealthSet::GetHealthAttribute()), 100.f);
			TestTrue(TEXT("【初始化-GE】PostExecute 应触发"),
				Env.HealthSet->bDebugPostExecuteFired);
		});
	});

	Describe(TEXT("A04 GameplayTag 层级匹配"), [this]()
	{
		It(TEXT("子 Tag MatchesTag 父 Tag 应为 true"), [this]()
		{
			FGameplayTag Fireball = MyGameplayTags::TAG_Ability_Skill_Fireball;
			FGameplayTag AbilityRoot = FGameplayTag::RequestGameplayTag(TEXT("Ability"));
			TestTrue(TEXT("【Tag层级】Fireball.MatchesTag(Ability)"),
				Fireball.MatchesTag(AbilityRoot));
			TestFalse(TEXT("【Tag精确】Fireball.MatchesTagExact(Ability) 应为 false"),
				Fireball.MatchesTagExact(AbilityRoot));
		});

		It(TEXT("Container HasAny 匹配子树"), [this]()
		{
			FGameplayTagContainer C;
			C.AddTag(MyGameplayTags::TAG_Ability_Skill_Fireball);
			C.AddTag(MyGameplayTags::TAG_State_Stunned);
			TestEqual(TEXT("【Tag容器】容器大小应为 2"), C.Num(), 2);
			TestTrue(TEXT("【Tag容器】HasAny(Ability) 应为 true"),
				C.HasAny(FGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("Ability")))));
		});
	});

	Describe(TEXT("A05 PreAttributeChange Clamp"), [this]()
	{
		It(TEXT("超 MaxHealth 的设值应被 Clamp"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.HealthSet->InitHealth(50.f);
			Env.HealthSet->InitMaxHealth(100.f);

			Env.HealthSet->bDebugPreChangeFired = false;
			Env.ASC->SetNumericAttributeBase(UHealthSet::GetHealthAttribute(), 200.f);
			TestEqual(TEXT("【Clamp】超出应钳制到 MaxHealth=100"),
				Env.ASC->GetNumericAttribute(UHealthSet::GetHealthAttribute()), 100.f);
			TestTrue(TEXT("【Clamp】PreAttributeChange 应触发"),
				Env.HealthSet->bDebugPreChangeFired);
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
