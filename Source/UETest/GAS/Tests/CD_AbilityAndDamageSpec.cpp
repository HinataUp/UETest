// C / D 章节 自动化测试 ：GA 反射 + 伤害管线

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "GASTestSpecUtils.h"
#include "../C_GameplayAbility/C15_GA_TestFire.h"
#include "../C_GameplayAbility/C16_InstancingPolicy.h"
#include "../C_GameplayAbility/C17_NetExecPolicy.h"
#include "../C_GameplayAbility/C19_FireballCostCD.h"
#include "../C_GameplayAbility/C20_GA_TagFiveSet.h"
#include "../D_DamagePipeline/D22_GE_DamageMeta.h"
#include "../00_Common/MyGameplayTags.h"

BEGIN_DEFINE_SPEC(
	FUETest_GAS_CD_Spec,
	"UETest.GAS.C_GameplayAbility与D_伤害",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_GAS_CD_Spec)

void FUETest_GAS_CD_Spec::Define()
{
	using namespace GASTestSpecUtils;

	Describe(TEXT("C15-C16 GA 默认配置"), [this]()
	{
		It(TEXT("UGA_TestFire 默认 InstancedPerActor + LocalPredicted"), [this]()
		{
			UGA_TestFire* GA = NewObject<UGA_TestFire>();
			TestEqual(TEXT("【GA】InstancingPolicy"),
				static_cast<int32>(GA->GetInstancingPolicy()),
				static_cast<int32>(EGameplayAbilityInstancingPolicy::InstancedPerActor));
			TestEqual(TEXT("【GA】NetExecutionPolicy"),
				static_cast<int32>(GA->GetNetExecutionPolicy()),
				static_cast<int32>(EGameplayAbilityNetExecutionPolicy::LocalPredicted));
		});

		It(TEXT("UGA_PerExecution 配置正确"), [this]()
		{
			UGA_PerExecution* GA = NewObject<UGA_PerExecution>();
			TestEqual(TEXT("【GA】PerExecution"),
				static_cast<int32>(GA->GetInstancingPolicy()),
				static_cast<int32>(EGameplayAbilityInstancingPolicy::InstancedPerExecution));
		});
	});

	Describe(TEXT("C17 NetExecutionPolicy 四种"), [this]()
	{
		It(TEXT("ServerOnly / LocalOnly / LocalPredicted / ServerInitiated 各自正确"), [this]()
		{
			TestEqual(TEXT("LocalOnly"),       static_cast<int32>(NewObject<UGA_NetLocalOnly>()->GetNetExecutionPolicy()),       static_cast<int32>(EGameplayAbilityNetExecutionPolicy::LocalOnly));
			TestEqual(TEXT("LocalPredicted"),  static_cast<int32>(NewObject<UGA_NetLocalPredicted>()->GetNetExecutionPolicy()),  static_cast<int32>(EGameplayAbilityNetExecutionPolicy::LocalPredicted));
			TestEqual(TEXT("ServerOnly"),      static_cast<int32>(NewObject<UGA_NetServerOnly>()->GetNetExecutionPolicy()),      static_cast<int32>(EGameplayAbilityNetExecutionPolicy::ServerOnly));
			TestEqual(TEXT("ServerInitiated"), static_cast<int32>(NewObject<UGA_NetServerInitiated>()->GetNetExecutionPolicy()), static_cast<int32>(EGameplayAbilityNetExecutionPolicy::ServerInitiated));
		});
	});

	Describe(TEXT("C19 Fireball Cost / CD 绑定"), [this]()
	{
		It(TEXT("Cost / Cooldown GE 类已绑定"), [this]()
		{
			UGA_Fireball* GA = NewObject<UGA_Fireball>();
			TestNotNull(TEXT("【Cost】CostGameplayEffectClass"), GA->GetCostGameplayEffect());
			TestNotNull(TEXT("【CD】CooldownGameplayEffectClass"), GA->GetCooldownGameplayEffect());
		});
	});

	Describe(TEXT("C20 GA Tag 五件套"), [this]()
	{
		It(TEXT("Dash 的 Required / Blocked / Owned 都已配置"), [this]()
		{
			UGA_Dash* GA = NewObject<UGA_Dash>();
			TestTrue(TEXT("Required: OnGround"),
				GA->ActivationRequiredTags.HasTag(MyGameplayTags::TAG_State_OnGround));
			TestTrue(TEXT("Blocked: Stunned"),
				GA->ActivationBlockedTags.HasTag(MyGameplayTags::TAG_State_Stunned));
			TestTrue(TEXT("Owned: Active.Dash"),
				GA->ActivationOwnedTags.HasTag(MyGameplayTags::TAG_Ability_Active_Dash));
		});

		It(TEXT("Fireball Cancel Movement"), [this]()
		{
			UGA_FireballSkill* GA = NewObject<UGA_FireballSkill>();
			TestTrue(TEXT("Cancel: Movement"),
				GA->CancelAbilitiesWithTag.HasTag(MyGameplayTags::TAG_Ability_Movement));
		});
	});

	Describe(TEXT("D22 Meta Attribute 伤害管线"), [this]()
	{
		It(TEXT("通过 IncomingDamage 写入扣血 ，IncomingDamage 自动重置"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.HealthSet->InitHealth(100.f);

			FGameplayEffectContextHandle Ctx = Env.ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = Env.ASC->MakeOutgoingSpec(
				UGE_DamageMeta::StaticClass(), 1.f, Ctx);
			Spec.Data->SetSetByCallerMagnitude(MyGameplayTags::TAG_SetByCaller_Damage, 30.f);
			Env.ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());

			TestEqual(TEXT("【Meta】Health 100-30=70"),
				Env.ASC->GetNumericAttribute(UHealthSet::GetHealthAttribute()), 70.f);
			TestEqual(TEXT("【Meta】IncomingDamage 用完归零"),
				Env.ASC->GetNumericAttribute(UHealthSet::GetIncomingDamageAttribute()), 0.f);
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
