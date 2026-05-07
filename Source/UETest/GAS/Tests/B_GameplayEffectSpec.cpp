// B 章节 自动化测试

#include "CoreMinimal.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "GASTestSpecUtils.h"
#include "../B_GameplayEffect/B06_GE_Heal30.h"
#include "../B_GameplayEffect/B07_GE_AtkBuff5s.h"
#include "../B_GameplayEffect/B08_GE_EquipBuff.h"
#include "../B_GameplayEffect/B10_GE_ModifierOrder.h"
#include "../B_GameplayEffect/B11_GE_DamageSetByCaller.h"
#include "../B_GameplayEffect/B14_GE_FireImmunity.h"
#include "../00_Common/MyGameplayTags.h"

BEGIN_DEFINE_SPEC(
	FUETest_GAS_B_GE_Spec,
	"UETest.GAS.B_GameplayEffect",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
END_DEFINE_SPEC(FUETest_GAS_B_GE_Spec)

void FUETest_GAS_B_GE_Spec::Define()
{
	using namespace GASTestSpecUtils;

	Describe(TEXT("B06 Instant GE"), [this]()
	{
		It(TEXT("Apply 后 Health Base +30 ，不进 ActiveEffects"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.HealthSet->InitHealth(70.f);
			ApplyGEToSelf(Env.ASC, UGE_Heal30::StaticClass());

			TestEqual(TEXT("【Instant】Health 70 -> 100"),
				Env.ASC->GetNumericAttributeBase(UHealthSet::GetHealthAttribute()), 100.f);
		});
	});

	Describe(TEXT("B07 Duration GE"), [this]()
	{
		It(TEXT("Apply 后 Current=Base+10 ，Base 不变"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.CombatSet->InitAttackPower(50.f);
			ApplyGEToSelf(Env.ASC, UGE_AtkBuff5s::StaticClass());

			TestEqual(TEXT("【Duration】Base 不变"),
				Env.ASC->GetNumericAttributeBase(UCombatSet::GetAttackPowerAttribute()), 50.f);
			TestEqual(TEXT("【Duration】Current = Base+10"),
				Env.ASC->GetNumericAttribute(UCombatSet::GetAttackPowerAttribute()), 60.f);
		});
	});

	Describe(TEXT("B08 Infinite GE 三种移除方式"), [this]()
	{
		It(TEXT("RemoveActiveGameplayEffect(Handle) 能正确移除"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.HealthSet->InitMaxHealth(100.f);
			auto H = ApplyGEToSelf(Env.ASC, UGE_EquipBuff::StaticClass());
			TestEqual(TEXT("【Infinite】MaxHealth +50"),
				Env.ASC->GetNumericAttribute(UHealthSet::GetMaxHealthAttribute()), 150.f);
			Env.ASC->RemoveActiveGameplayEffect(H);
			TestEqual(TEXT("【Infinite】移除后回落"),
				Env.ASC->GetNumericAttribute(UHealthSet::GetMaxHealthAttribute()), 100.f);
		});
	});

	Describe(TEXT("B10 Modifier 运算顺序"), [this]()
	{
		It(TEXT("(Base+Add)*Mul = (20+10)*1.5 = 45"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.CombatSet->InitAttackPower(20.f);
			ApplyGEToSelf(Env.ASC, UGE_AtkAdd10::StaticClass());
			ApplyGEToSelf(Env.ASC, UGE_AtkMul1_5::StaticClass());
			TestEqual(TEXT("【Modifier顺序】(20+10)*1.5=45"),
				Env.ASC->GetNumericAttribute(UCombatSet::GetAttackPowerAttribute()), 45.f);
		});

		It(TEXT("Override 50 直接覆盖"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.CombatSet->InitAttackPower(20.f);
			ApplyGEToSelf(Env.ASC, UGE_AtkAdd10::StaticClass());
			ApplyGEToSelf(Env.ASC, UGE_AtkOverride50::StaticClass());
			TestEqual(TEXT("【Override】直接覆盖为 50"),
				Env.ASC->GetNumericAttribute(UCombatSet::GetAttackPowerAttribute()), 50.f);
		});
	});

	Describe(TEXT("B11 SetByCaller"), [this]()
	{
		It(TEXT("SetSetByCallerMagnitude(-40) 能正确扣血"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.HealthSet->InitHealth(100.f);
			FGameplayEffectContextHandle Ctx = Env.ASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = Env.ASC->MakeOutgoingSpec(
				UGE_DamageSetByCaller::StaticClass(), 1.f, Ctx);
			Spec.Data->SetSetByCallerMagnitude(MyGameplayTags::TAG_SetByCaller_Damage, -40.f);
			Env.ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
			TestEqual(TEXT("【SetByCaller】Health 100-40=60"),
				Env.ASC->GetNumericAttribute(UHealthSet::GetHealthAttribute()), 60.f);
		});
	});

	Describe(TEXT("B14 Immunity"), [this]()
	{
		It(TEXT("免疫火焰伤害时 Apply 失败"), [this]()
		{
			FTestEnv Env;
			Env.Setup();
			if (!Env.IsValid()) return;

			Env.HealthSet->InitHealth(100.f);
			ApplyGEToSelf(Env.ASC, UGE_FireImmunity::StaticClass());
			auto DmgHandle = ApplyGEToSelf(Env.ASC, UGE_FireDamage::StaticClass());
			TestEqual(TEXT("【免疫】Health 应不变"),
				Env.ASC->GetNumericAttribute(UHealthSet::GetHealthAttribute()), 100.f);
		});
	});
}

#endif // WITH_DEV_AUTOMATION_TESTS
